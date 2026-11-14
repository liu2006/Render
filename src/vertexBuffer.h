#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include "renderer.h"
#include "config.h"

#if defined(VERTEX_BUFFER_IMPLEMENTATION)
    bool createVertexBuffer(Renderer *renderer)
    {
	Vulkan *vulkan = renderer->vulkan;

	Vertex vertices[] = {
	    { .position = { -0.5f, -0.5f }, .color = { 1.0f, 0.0f, 0.0f} },
	    { .position = { 0.5f, -0.5f }, .color = { 0.0f, 1.0f, 0.0f} },
	    { .position = { 0.5f, 0.5f }, .color = { 0.0f, 0.0f, 1.0f} },
	    { .position = { -0.5f, 0.5f }, .color = { 0.3f, 0.7f, 0.5f} },
	};

	VkBuffer stagingBuffer = VK_NULL_HANDLE;
	VkDeviceMemory stagingBufferMemory = VK_NULL_HANDLE;
	const VkBufferCreateInfo stagingBufferInfo = {
	    .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
	    .pNext = NULL,
	    .flags = 0,
	    .size = sizeof(vertices),
	    .usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
	    .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
	    .queueFamilyIndexCount = 0,
	    .pQueueFamilyIndices = NULL
	};
	vulkan->result = vkCreateBuffer(vulkan->device, &stagingBufferInfo, NULL, &stagingBuffer);
	if (vulkan->result != VK_SUCCESS) {
	    logMessage(LOG_ERROR, "Failed to create staging buffer\n");
	    goto clean;
	} 
	
	VkMemoryRequirements memoryRequirements;
	vkGetBufferMemoryRequirements(vulkan->device, stagingBuffer, &memoryRequirements);

	VkPhysicalDeviceMemoryProperties memoryProperties;
	vkGetPhysicalDeviceMemoryProperties(vulkan->physicalDevice, &memoryProperties);
	uint32_t stagingMemoryTypeIndex = UINT32_MAX;
	for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; ++i) {
	    if (!(memoryRequirements.memoryTypeBits & (1u << i)))
		continue;
	    if (!(memoryProperties.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT))
		continue;
	    if (!(memoryProperties.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT))
		continue;
	    stagingMemoryTypeIndex = i;
	    break;
	}
	if (stagingMemoryTypeIndex == UINT32_MAX) {
	    logMessage(LOG_ERROR, "Failed to find suitable staging memory type\n");
	    goto clean;
	}
	const VkMemoryAllocateInfo allocateInfo = {
	    .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
	    .pNext = 0,
	    .allocationSize = memoryRequirements.size,
	    .memoryTypeIndex = stagingMemoryTypeIndex
	};
	
	vulkan->result = vkAllocateMemory(vulkan->device, &allocateInfo, NULL, &stagingBufferMemory);
	if (vulkan->result != VK_SUCCESS) {
	    logMessage(LOG_ERROR, "Failed to allocate memory for staging buffer\n");
	    goto clean;
	}
	vulkan->result = vkBindBufferMemory(vulkan->device, stagingBuffer, stagingBufferMemory, 0);
	if (vulkan->result != VK_SUCCESS) {
	    logMessage(LOG_ERROR, "Failed to bind memory for staging buffer\n");
	    goto clean;
	}
	
	void *data;
	vulkan->result = vkMapMemory(vulkan->device, stagingBufferMemory, 0, memoryRequirements.size, 0, &data);
	if (vulkan->result != VK_SUCCESS) {
	    logMessage(LOG_ERROR, "Failed to map stagging buffer memory\n");
	}
	memcpy(data, vertices, stagingBufferInfo.size);
	
	vkUnmapMemory(vulkan->device, stagingBufferMemory);
	
	vulkan->result = VK_SUCCESS;
 clean:
	if (stagingBuffer != VK_NULL_HANDLE) {
	    vkDestroyBuffer(vulkan->device, stagingBuffer, NULL);
	}
	vkFreeMemory(vulkan->device, stagingBufferMemory, NULL);
	return (vulkan->result != VK_SUCCESS) ? false : true;
    }

#endif

#endif
