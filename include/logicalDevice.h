#ifndef LOGICAL_DEVICE_H
#define LOGICAL_DEVICE_H

#include "vulkan.h"
#include "physicalDevice.h"

typedef struct {
    void *ptr; 
} LogicalDevice;

bool createLogicalDevice(Vulkan *vulkan, LogicalDevice *logicalDevice, PhysicalDevice *physicalDevice);

#if defined(LOGICAL_DEVICE_IMPLEMENTATION)
    bool createLogicalDevice(Vulkan *vulkan, LogicalDevice *logicalDevice, PhysicalDevice *physicalDevice)
    {
	(void)logicalDevice;
	float queuePriority = 1.0f;
	const VkDeviceQueueCreateInfo queueInfo = {
	    .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO, 
	    .pNext = NULL,
	    .flags = 0,
	    .queueFamilyIndex = physicalDevice->graphicsIndex,
	    .queueCount = 1,
	    .pQueuePriorities = &queuePriority
	};
	VkPhysicalDeviceExtendedDynamicStateFeaturesEXT dynamicState = {
	    .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_DYNAMIC_STATE_FEATURES_EXT,
	    .pNext = NULL,
	    .extendedDynamicState = VK_TRUE
	};
	VkPhysicalDeviceVulkan13Features features13 = {
	    .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES,
	    .pNext = &dynamicState,
	    .dynamicRendering = VK_TRUE,
	};
	VkPhysicalDeviceVulkan11Features features11 = {
	    .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES,
	    .pNext = &features13,
	    .shaderDrawParameters = VK_TRUE,
	};    
	VkPhysicalDeviceFeatures2 features2 = {
	    .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2,
	    .pNext = &features11,
	};

	const VkDeviceCreateInfo deviceInfo = {
	    .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
	    .pNext = &features2,
	    .flags = 0,
	    .queueCreateInfoCount = 1,
	    .pQueueCreateInfos = &queueInfo,
	    .enabledLayerCount = 0,
	    .ppEnabledLayerNames = NULL,
	    .enabledExtensionCount = physicalDevice->deviceExtensionCount,
	    .ppEnabledExtensionNames = physicalDevice->deviceExtensions,
	    .pEnabledFeatures = NULL
	};
    
	vulkan->result = vkCreateDevice(vulkan->physicalDevice, &deviceInfo, NULL, &vulkan->device);
	if (vulkan->result != VK_SUCCESS) {
	    logMessage(LOG_ERROR, "Failed to create logical device [%]\n", vulkan->result);
	    return false;
	}
	logMessage(LOG_INFO, "Logical device created\n");
	vkGetDeviceQueue(vulkan->device, physicalDevice->graphicsIndex, 0, &vulkan->graphicsQueue);
	return true;
    }
#endif
    
#endif
