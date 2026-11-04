#include "instance.h"

bool createInstance(Vulkan *vulkan, Instance *instance)
{
    uint32_t layerPropertyCount;
    VkLayerProperties *layerProperties = NULL;
    uint32_t extensionPropertyCount;
    VkExtensionProperties *extensionProperties = NULL;

#if ENABLE_VALIDATION
    instance->layerCount = sizeof(requiredLayers) / sizeof(requiredLayers[0]);
    instance->layers = malloc(sizeof(*instance->layers) * instance->layerCount);
    for (uint32_t i = 0; i < instance->layerCount; ++i) {
	instance->layers[i] = strdup(requiredLayers[i]);
    }

    vulkan->result = vkEnumerateInstanceLayerProperties(&layerPropertyCount, NULL);
    if (vulkan->result != VK_SUCCESS) {
	logMessage(LOG_ERROR, "Failed to enumerate instance layer properties [%d]\n", vulkan->result);
	goto clean;
    }

    layerProperties = malloc(sizeof(*layerProperties) * layerPropertyCount);
    vulkan->result = vkEnumerateInstanceLayerProperties(&layerPropertyCount, layerProperties);
    if (vulkan->result != VK_SUCCESS) {
	logMessage(LOG_ERROR, "Failed to enumerate instance layer properties [%d]\n", vulkan->result);
	goto clean;
    }
    for (uint32_t i = 0; i < instance->layerCount; ++i) {
	bool found = false;
	for (uint32_t j = 0; j < layerPropertyCount; ++j) {
	    logMessage(LOG_INFO, "Available layer: [%s]\n", layerProperties[j].layerName);
	    if (strcmp(layerProperties[j].layerName, instance->layers[i]) == 0) {
		found = true;
		break;
	    }
	}
	if (!found) {
	    logMessage(LOG_ERROR, "Required layer is not supported: [%s]\n", instance->layers[i]);
	    vulkan->result = VK_ERROR_LAYER_NOT_PRESENT;
	    goto clean;
	}
    }
#endif

    instance->extensions = glfwGetRequiredInstanceExtensions(&instance->extensionCount);
    vulkan->result = vkEnumerateInstanceExtensionProperties(NULL, &extensionPropertyCount, NULL);
    if (vulkan->result != VK_SUCCESS) {
	logMessage(LOG_ERROR, "Failed to enumerate instance extension properties [%d]\n", vulkan->result);
	goto clean;
    }

    extensionProperties = malloc(sizeof(*extensionProperties) * extensionPropertyCount);
    vulkan->result = vkEnumerateInstanceExtensionProperties(NULL, &extensionPropertyCount, extensionProperties);
    if (vulkan->result != VK_SUCCESS) {
	logMessage(LOG_ERROR, "Failed to enumerate instance extension properties [%d]\n", vulkan->result);
	goto clean;
    }

    for (uint32_t i = 0; i < instance->extensionCount; ++i) {
	bool found = false;
	for (uint32_t j = 0; j < extensionPropertyCount; ++j) {
	    if (strcmp(extensionProperties[j].extensionName, instance->extensions[i]) == 0) {
		found = true;
		logMessage(LOG_INFO, "GLFW extension is supported: [%s]\n", instance->extensions[i]);
		break;
	    }
	}
	if (!found) {
	    logMessage(LOG_ERROR, "GLFW required extension is not supported: [%s]\n", instance->extensions[i]);
	    vulkan->result = VK_ERROR_EXTENSION_NOT_PRESENT;
	    goto clean;
	}
    }
    const VkApplicationInfo appInfo = {
	.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
	.pNext = NULL,
	.pApplicationName = "render",
	.applicationVersion = VK_MAKE_VERSION(1, 0, 0),
	.pEngineName = "No Engine",
	.engineVersion = VK_MAKE_VERSION(1, 0, 0),
	.apiVersion = VK_API_VERSION_1_4
    };
    
    const VkInstanceCreateInfo instanceInfo = {
	.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
	.pNext = NULL,
	.flags = 0,  
	.pApplicationInfo = &appInfo,
	.enabledLayerCount = instance->layerCount, 
	.ppEnabledLayerNames = instance->layers,
	.enabledExtensionCount = instance->extensionCount,
	.ppEnabledExtensionNames = instance->extensions
    };

    vulkan->result = vkCreateInstance(&instanceInfo, NULL, &vulkan->instance);
    if (vulkan->result != VK_SUCCESS) {
	logMessage(LOG_ERROR, "Failed to create vulkan instance [%d]\n", vulkan->result);
	goto clean;
    }
    logMessage(LOG_INFO, "Vulkan instance created\n");
    vulkan->result = VK_SUCCESS;

 clean:
    free(layerProperties);
    free(extensionProperties);

    return (vulkan->result == VK_SUCCESS) ? true : false; 
}

