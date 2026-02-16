#pragma once

#include <vulkan/vulkan_core.h>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <glm/glm.hpp>
#include <vector>
#include <vulkan/vulkan.h>

namespace lve {
class LveModel {
public:
  struct Vertex {
    glm::vec2 position;
    glm::vec3 color;
    static std::vector<VkVertexInputBindingDescription> getBindingDescription();
    static std::vector<VkVertexInputAttributeDescription>
    getAttributeDescription();
  };

  LveModel(VkPhysicalDevice physicalDevice, VkDevice device,
           const std::vector<Vertex> &vertices);
  ~LveModel();

  LveModel(const LveModel &) = delete;
  LveModel &operator=(const LveModel &) = delete;

  void bind(VkCommandBuffer commandBuffer);
  void draw(VkCommandBuffer commandBuffer);

private:
  void createVertexBuffers(const std::vector<Vertex> &vertices);
  uint32_t findMemoryType(uint32_t typeFiler, VkMemoryPropertyFlags properties);
  VkPhysicalDevice lvePhysicalDevice;
  VkDevice lveDevice;
  VkBuffer vertexBuffer;
  VkDeviceMemory vertexBufferMemory;
  uint32_t vertexCount;
};
} // namespace lve
