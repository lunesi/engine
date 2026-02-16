#include "VulcanApp.hpp"
#include <algorithm>
#include <cstdint>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <vector>

// --- ИНИЦИАЛИЗАЦИЯ ОКНА ---
void VulcanApp::initWindow() {
  if (!glfwInit()) {
    throw std::runtime_error("Failed to initialize GLFW");
  }

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // Отключаем OpenGL
  glfwWindowHint(GLFW_RESIZABLE,
                 GLFW_FALSE); // Фиксированный размер для стабильности
  glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE); // Явно просим показать окно

  window = glfwCreateWindow(800, 600, "Vulkan Engine - Pure", nullptr, nullptr);

  if (!window) {
    throw std::runtime_error("Failed to create GLFW Window");
  }

  // Небольшой "пинок" для оконного менеджера Linux
  glfwShowWindow(window);
  glfwFocusWindow(window);
}

// --- ИНИЦИАЛИЗАЦИЯ VULKAN ---
void VulcanApp::initVulkan() {
  createInstance();
  createSurface(); // Поверхность СРАЗУ после инстанса
  pickPhysicalDevice();
  createLogicalDevice();
  createSwapChain();
  createImageViews();
  createRenderPass();
  createFramebuffers();
  createCommandPool();
  createCommandBuffers();
}

void VulcanApp::createInstance() {
  VkApplicationInfo appInfo{};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.apiVersion = VK_API_VERSION_1_0;

  VkInstanceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;

  uint32_t glfwExtensionCount = 0;
  const char **glfwExtensions =
      glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
  createInfo.enabledExtensionCount = glfwExtensionCount;
  createInfo.ppEnabledExtensionNames = glfwExtensions;

  if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
    throw std::runtime_error("failed to create instance!");
  }
}

void VulcanApp::pickPhysicalDevice() {
  uint32_t deviceCount = 0;
  vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
  if (deviceCount == 0)
    throw std::runtime_error("failed to find GPUs with Vulkan support!");

  std::vector<VkPhysicalDevice> devices(deviceCount);
  vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

  physicalDevice = devices[0];
  vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);
  std::cout << "🚀 Using GPU: " << deviceProperties.deviceName << std::endl;
}

void VulcanApp::createLogicalDevice() {
  uint32_t graphicsFamily = findGraphicsQueueFamily();
  float queuePriority = 1.0f;

  VkDeviceQueueCreateInfo queueCreateInfo{};
  queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
  queueCreateInfo.queueFamilyIndex = graphicsFamily;
  queueCreateInfo.queueCount = 1;
  queueCreateInfo.pQueuePriorities = &queuePriority;

  const std::vector<const char *> deviceExtensions = {
      VK_KHR_SWAPCHAIN_EXTENSION_NAME};

  VkDeviceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  createInfo.queueCreateInfoCount = 1;
  createInfo.pQueueCreateInfos = &queueCreateInfo;
  createInfo.enabledExtensionCount =
      static_cast<uint32_t>(deviceExtensions.size());
  createInfo.ppEnabledExtensionNames = deviceExtensions.data();

  if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to create logical device!");
  }
  vkGetDeviceQueue(device, graphicsFamily, 0, &graphicsQueue);
}

void VulcanApp::createSurface() {
  if (glfwCreateWindowSurface(instance, window, nullptr, &surface) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to create window surface!");
  }
}

void VulcanApp::createSwapChain() {
  VkSurfaceCapabilitiesKHR capabilities;
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface,
                                            &capabilities);

  if (capabilities.currentExtent.width !=
      std::numeric_limits<uint32_t>::max()) {
    swapChainExtent = capabilities.currentExtent;
  } else {
    swapChainExtent = {800, 600};
  }

  swapChainImageFormat = VK_FORMAT_B8G8R8A8_UNORM;

  uint32_t imageCount = capabilities.minImageCount + 1;
  if (capabilities.maxImageCount > 0 &&
      imageCount > capabilities.maxImageCount) {
    imageCount = capabilities.maxImageCount;
  }

  VkSwapchainCreateInfoKHR createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  createInfo.surface = surface;
  createInfo.minImageCount = imageCount;
  createInfo.imageFormat = swapChainImageFormat;
  createInfo.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
  createInfo.imageExtent = swapChainExtent;
  createInfo.imageArrayLayers = 1;
  createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
  createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
  createInfo.preTransform = capabilities.currentTransform;
  createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  createInfo.presentMode = VK_PRESENT_MODE_FIFO_KHR; // V-Sync
  createInfo.clipped = VK_TRUE;

  if (vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapChain) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to create swap chain!");
  }

  vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);
  swapChainImage.resize(imageCount);
  vkGetSwapchainImagesKHR(device, swapChain, &imageCount,
                          swapChainImage.data());
}

void VulcanApp::createImageViews() {
  swapChainImageViews.resize(swapChainImage.size());
  for (size_t i = 0; i < swapChainImage.size(); i++) {
    VkImageViewCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    createInfo.image = swapChainImage[i];
    createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    createInfo.format = swapChainImageFormat;
    createInfo.subresourceRange = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1};
    vkCreateImageView(device, &createInfo, nullptr, &swapChainImageViews[i]);
  }
}

void VulcanApp::createRenderPass() {
  VkAttachmentDescription colorAttachment{};
  colorAttachment.format = swapChainImageFormat;
  colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
  colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

  VkAttachmentReference colorRef{0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL};

  VkSubpassDescription subpass{};
  subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
  subpass.colorAttachmentCount = 1;
  subpass.pColorAttachments = &colorRef;

  VkRenderPassCreateInfo renderPassInfo{};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  renderPassInfo.attachmentCount = 1;
  renderPassInfo.pAttachments = &colorAttachment;
  renderPassInfo.subpassCount = 1;
  renderPassInfo.pSubpasses = &subpass;

  if (vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to create render pass!");
  }
}

void VulcanApp::createFramebuffers() {
  swapChainFramebuffers.resize(swapChainImageViews.size());
  for (size_t i = 0; i < swapChainImageViews.size(); i++) {
    VkImageView attachments[] = {swapChainImageViews[i]};
    VkFramebufferCreateInfo fbInfo{};
    fbInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    fbInfo.renderPass = renderPass;
    fbInfo.attachmentCount = 1;
    fbInfo.pAttachments = attachments;
    fbInfo.width = swapChainExtent.width;
    fbInfo.height = swapChainExtent.height;
    fbInfo.layers = 1;
    vkCreateFramebuffer(device, &fbInfo, nullptr, &swapChainFramebuffers[i]);
  }
}

void VulcanApp::createCommandPool() {
  VkCommandPoolCreateInfo poolInfo{};
  poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
  poolInfo.queueFamilyIndex = findGraphicsQueueFamily();
  vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool);
}

void VulcanApp::createCommandBuffers() {
  commandBuffers.resize(swapChainImage.size());
  VkCommandBufferAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.commandPool = commandPool;
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();
  vkAllocateCommandBuffers(device, &allocInfo, commandBuffers.data());
}

// --- ОТРИСОВКА ---
void VulcanApp::drawFrame() {
  uint32_t imageIndex;
  // Ожидаем изображение из свопчейна
  VkResult result =
      vkAcquireNextImageKHR(device, swapChain, UINT64_MAX, VK_NULL_HANDLE,
                            VK_NULL_HANDLE, &imageIndex);

  if (result == VK_ERROR_OUT_OF_DATE_KHR)
    return;
  if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
    throw std::runtime_error("failed to acquire swap chain image!");
  }

  vkResetCommandBuffer(commandBuffers[imageIndex], 0);
  VkCommandBufferBeginInfo beginInfo{
      VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};
  vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo);

  VkRenderPassBeginInfo rpInfo{};
  rpInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  rpInfo.renderPass = renderPass;
  rpInfo.framebuffer = swapChainFramebuffers[imageIndex];
  rpInfo.renderArea.extent = swapChainExtent;

  VkClearValue clearColor = {{{0.1f, 0.1f, 0.1f, 1.0f}}};
  rpInfo.clearValueCount = 1;
  rpInfo.pClearValues = &clearColor;

  vkCmdBeginRenderPass(commandBuffers[imageIndex], &rpInfo,
                       VK_SUBPASS_CONTENTS_INLINE);
  vkCmdEndRenderPass(commandBuffers[imageIndex]);

  if (vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS) {
    throw std::runtime_error("failed to record command buffer!");
  }

  VkSubmitInfo submitInfo{VK_STRUCTURE_TYPE_SUBMIT_INFO};
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &commandBuffers[imageIndex];

  vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);

  VkPresentInfoKHR presentInfo{VK_STRUCTURE_TYPE_PRESENT_INFO_KHR};
  presentInfo.swapchainCount = 1;
  presentInfo.pSwapchains = &swapChain;
  presentInfo.pImageIndices = &imageIndex;

  vkQueuePresentKHR(graphicsQueue, &presentInfo);

  // Важно для стабильности на AMD: ждем завершения операций на GPU
  vkDeviceWaitIdle(device);
}

void VulcanApp::mainLoop() {
  std::cout << "--- Main Loop Started ---" << std::endl;
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    drawFrame();
  }
  std::cout << "--- Main Loop Ended ---" << std::endl;
}

void VulcanApp::cleanup() {
  vkDeviceWaitIdle(device);

  for (auto fb : swapChainFramebuffers)
    vkDestroyFramebuffer(device, fb, nullptr);
  vkDestroyRenderPass(device, renderPass, nullptr);
  for (auto iv : swapChainImageViews)
    vkDestroyImageView(device, iv, nullptr);
  vkDestroyCommandPool(device, commandPool, nullptr);
  vkDestroySwapchainKHR(device, swapChain, nullptr);
  vkDestroyDevice(device, nullptr);
  vkDestroySurfaceKHR(instance, surface, nullptr);
  vkDestroyInstance(instance, nullptr);

  glfwDestroyWindow(window);
  glfwTerminate();
}

uint32_t VulcanApp::findGraphicsQueueFamily() {
  uint32_t count = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &count, nullptr);
  std::vector<VkQueueFamilyProperties> families(count);
  vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &count,
                                           families.data());
  for (uint32_t i = 0; i < count; i++) {
    if (families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
      return i;
  }
  return 0;
}
