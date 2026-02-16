#pragma once
#include "lve_model.hpp"
#include "lve_pipeline.hpp"
#include "lve_window.hpp"
#include <cstdint>
#include <memory>
#include <vector>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

class VulcanApp {
public:
  void run() {
    initVulkan();
    mainLoop();
    cleanup();
  }

private:
  lve::LveWindow lveWindow{WIDTH, HEIGHT, "Vulkan Engine"};
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
  std::unique_ptr<lve::LvePipeline> LvePipeline;
  void createGraphicsPipeline();
  static std::vector<char> readFile(const std::string &fileName);
  VkShaderModule createShaderModule(const std::vector<char> &code);

  std::unique_ptr<lve::LveModel> lveModel;

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
  void createPipelineLayout();
  void loadModels();
  void createFramebuffers();
  void createCommandPool();
  void createCommandBuffers();
  void drawFrame();
  void initImgui();

  uint32_t findGraphicsQueueFamily();
};
