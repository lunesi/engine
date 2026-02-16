#pragma once
#include <vulkan/vulkan_core.h>
#define GLFW_INCLUDE_VULCAN
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"
#include <GLFW/glfw3.h>
#include <cstdint>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <vector>
#include <vulkan/vulkan.h>
class VulcanApp {
public:
  void run() {
    initWindow();
    initVulkan();
    mainLoop();
    cleanup();
  }

private:
  GLFWwindow *window;
  VkInstance instance;
  VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
  VkDevice device;
  VkQueue graphicsQueue;
  VkSurfaceKHR surface;
  VkPhysicalDeviceProperties deviceProperties;
  VkSwapchainKHR swapChain;
  std::vector<VkImage> swapChainImage;
  VkFormat swapChainImageFormat;
  VkExtent2D swapChainExtent;
  std::vector<VkImageView> swapChainImageView;
  std::vector<VkImageView> swapChainImageViews;
  VkRenderPass renderPass;
  std::vector<VkFramebuffer> swapChainFramebuffers;
  VkCommandPool commandPool;
  std::vector<VkCommandBuffer> commandBuffers;
  VkDescriptorPool imguiPool;

  VkPipelineLayout pipelineLayout;
  VkPipeline graphicsPipeline;
  void createGraphicsPipeline();
  static std::vector<char> readFile(const std::string &fileName);
  VkShaderModule createShaderModule(const std::vector<char> &code);

  void initWindow();
  void initVulkan();
  void mainLoop();
  void cleanup();

  void createInstance();
  void pickPhysicalDevice();
  void createLogicalDevice();
  void createSurface();
  void createSwapChain();
  void createImageView();
  void createImageViews();
  void createRenderPass();
  void createFramebuffers();
  void createCommandPool();
  void createCommandBuffers();
  void drawFrame();
  void initImgui();

  uint32_t findGraphicsQueueFamily();
};
