#include "physicalDevice.h"

bool pickPhysicalDevice(Vulkan *vulkan, PhysicalDevice *physicalDevice) 
{
    uint32_t physicalDeviceCount;
    VkPhysicalDevice *physicalDevices = NULL;    

    uint32_t queueFamilyPropertyCount;
    VkQueueFamilyProperties *queueFamilyProperties = NULL;

    uint32_t extensionPropertyCount;
    VkExtensionProperties *extensionProperties = NULL;

    vulkan->result = vkEnumeratePhysicalDevices(vulkan->instance, &physicalDeviceCount, NULL);
    
    if (vulkan->result != VK_SUCCESS || physicalDeviceCount == 0) {
	if (vulkan->result == VK_SUCCESS) vulkan->result = VK_ERROR_INITIALIZATION_FAILED;
	logMessage(LOG_ERROR, "Physical Device not found [%d]\n", vulkan->result);
	goto clean;
    }
    physicalDevices = malloc(sizeof(*physicalDevices) * physicalDeviceCount);

    vulkan->result = vkEnumeratePhysicalDevices(vulkan->instance, &physicalDeviceCount, physicalDevices);
    if (vulkan->result != VK_SUCCESS || physicalDeviceCount == 0) {
	if (vulkan->result == VK_SUCCESS) vulkan->result = VK_ERROR_INITIALIZATION_FAILED;
	logMessage(LOG_ERROR, "Physical Device not found [%d]\n", vulkan->result);
	goto clean;
    }

    VkPhysicalDeviceProperties physicalDeviceProperties;
    for (uint32_t i = 0; i < physicalDeviceCount; ++i) {
	vkGetPhysicalDeviceProperties(physicalDevices[i], &physicalDeviceProperties);
	logMessage(LOG_INFO, "Available GPU: [%s]\n", physicalDeviceProperties.deviceName);
	if (physicalDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
	    vulkan->physicalDevice = physicalDevices[i];
	    break;
	}
	vulkan->physicalDevice = physicalDevices[i];
    }
    vkGetPhysicalDeviceProperties(vulkan->physicalDevice, &physicalDeviceProperties);
    logMessage(LOG_INFO, "Selected GPU: [%s]\n", physicalDeviceProperties.deviceName);

    vkGetPhysicalDeviceQueueFamilyProperties(vulkan->physicalDevice, &queueFamilyPropertyCount, NULL);
    queueFamilyProperties = malloc(sizeof(*queueFamilyProperties) * queueFamilyPropertyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(vulkan->physicalDevice, &queueFamilyPropertyCount, queueFamilyProperties);
    bool supportGraphics = false;
    for (uint32_t i = 0; i < queueFamilyPropertyCount; ++i) {
	if (queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
	    supportGraphics = true;
	    physicalDevice->graphicsIndex = i;
	    break;
	}
    }
    if (!supportGraphics) {
	vulkan->result = VK_ERROR_INITIALIZATION_FAILED;
	logMessage(LOG_ERROR, "[%s]: Graphics queue not supported\n", physicalDeviceProperties.deviceName);
	goto clean;
    }

    vulkan->result = vkEnumerateDeviceExtensionProperties(vulkan->physicalDevice, NULL, &extensionPropertyCount, NULL);
    if (vulkan->result != VK_SUCCESS) {
	logMessage(LOG_ERROR, "Failed to enumerate device extension properties [%d]\n", vulkan->result);
	goto clean;
    }
    extensionProperties = malloc(sizeof(*extensionProperties) * extensionPropertyCount); 
    vulkan->result = vkEnumerateDeviceExtensionProperties(vulkan->physicalDevice, NULL, &extensionPropertyCount, extensionProperties);
    if (vulkan->result != VK_SUCCESS) {
	logMessage(LOG_ERROR, "Failed to enumerate device extension properties [%d]\n", vulkan->result);
	goto clean;
    }

    physicalDevice->deviceExtensionCount = sizeof(requiredDeviceExtensions) / sizeof(requiredDeviceExtensions[0]);
    physicalDevice->deviceExtensions = malloc(sizeof(*physicalDevice->deviceExtensions) * physicalDevice->deviceExtensionCount);
    for (uint32_t i = 0; i < physicalDevice->deviceExtensionCount; ++i) {
	physicalDevice->deviceExtensions[i] = strdup(requiredDeviceExtensions[i]);
    }

    for (uint32_t i = 0; i < physicalDevice->deviceExtensionCount; ++i) {
	bool found = false;
	for (uint32_t j = 0; j < extensionPropertyCount; ++j) {
	    logMessage(LOG_INFO, "Available device extension: [%s]\n", extensionProperties[j].extensionName);
	    if (strcmp(extensionProperties[j].extensionName, 
		       physicalDevice->deviceExtensions[i]) == 0) {
		logMessage(LOG_INFO, "[%s]: Device extension is supported: [%s]\n", 
			   physicalDeviceProperties.deviceName,
			   requiredDeviceExtensions[i]);
		found = true;
		break;
	    }
	}
	if (!found) {
	    vulkan->result = VK_ERROR_INITIALIZATION_FAILED;
	    logMessage(LOG_ERROR, "[%s]: Required physical device extension not supported: [%s]\n", 
		       physicalDeviceProperties.deviceName,
		       requiredDeviceExtensions[i]);
	    goto clean;
	}
    }
    vulkan->result = VK_SUCCESS;
 clean:
    free(queueFamilyProperties);
    free(extensionProperties);
    free(physicalDevices);

    return (vulkan->result == VK_SUCCESS) ? true : false; 
}
