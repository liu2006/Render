#ifndef INSTANCE_H
#define INSTANCE_H

#include "vulkan.h"

#if defined(INSTANCE_IMPLEMENTATION)
    void *enableLayers(void *arg)
    {
	LayerTask *layerTask = (LayerTask *)arg;
	Instance *instance = layerTask->instance;
	VkResult localRes = layerTask->taskRes;

	uint32_t layerPropertyCount;
	VkLayerProperties *layerProperties = NULL;
	
#if ENABLE_VALIDATION
	instance->layerCount = sizeof(requiredLayers) / sizeof(requiredLayers[0]);
	instance->layers = malloc(sizeof(*instance->layers) * instance->layerCount);
	for (uint32_t i = 0; i < instance->layerCount; ++i) {
	    instance->layers[i] = strdup(requiredLayers[i]);
	}
	localRes = vkEnumerateInstanceLayerProperties(&layerPropertyCount, NULL);
	if (localRes != VK_SUCCESS) {
	    logMessage(LOG_ERROR, "Failed to enumerate instance layer properties [%d]\n", localRes);
	    goto clean;
	}
	layerProperties = malloc(sizeof(*layerProperties) * layerPropertyCount);
	localRes = vkEnumerateInstanceLayerProperties(&layerPropertyCount, layerProperties);
	if (localRes != VK_SUCCESS) {
	    logMessage(LOG_ERROR, "Failed to enumerate instance layer properties [%d]\n", localRes);
	    goto clean;
	}
	for (uint32_t i = 0; i < layerPropertyCount; ++i) {
	    logMessage(LOG_INFO, "Available layer: [%s]\n", layerProperties[i].layerName);	
	}

	for (uint32_t i = 0; i < instance->layerCount; ++i) {
	    bool found = false;
	    for (uint32_t j = 0; j < layerPropertyCount; ++j) {
		if (strcmp(layerProperties[j].layerName, instance->layers[i]) == 0) {
		    logMessage(LOG_INFO, "Enabled layer: [%s]\n", layerProperties[j].layerName);
		    found = true;
		    break;
		}
	    }
	    if (!found) {
		logMessage(LOG_ERROR, "Required layer is not supported: [%s]\n", instance->layers[i]);
		localRes = VK_ERROR_LAYER_NOT_PRESENT;
		goto clean;
	    }
	}
    clean:
	free(layerProperties);
#else
	(void)instance;
	(void)layerPropertyCount;
	(void)layerProperties;
	logMessage(LOG_INFO, "Layer not enabled\n");    
#endif
	layerTask->taskRes = localRes;
	return NULL;
    }

    int createInstance(Renderer *renderer)
    {
	Vulkan *vulkan = renderer->vulkan;
	Instance *instance = renderer->instance;

	uint32_t extensionPropertyCount;
	VkExtensionProperties *extensionProperties = NULL;
    
	LayerTask layerTask = {
	    .instance = instance,
	    .taskRes = VK_SUCCESS
	};
	pthread_t layerThread; 
	bool joined = false;
    
	if (pthread_create(&layerThread, NULL, enableLayers, &layerTask) != 0) {
	    logMessage(LOG_ERROR, "Failed to create layer thread\n");
	    vulkan->result = VK_ERROR_INITIALIZATION_FAILED; 
	    goto clean; 
	}
	
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
	for (uint32_t i = 0; i < extensionPropertyCount; ++i) { 
	    logMessage(LOG_INFO, "Available instance extension: [%s]\n", extensionProperties[i].extensionName);
	}

	for (uint32_t i = 0; i < instance->extensionCount; ++i) {
	    bool found = false;
	    for (uint32_t j = 0; j < extensionPropertyCount; ++j) {
		if (strcmp(extensionProperties[j].extensionName, instance->extensions[i]) == 0) {
    		    logMessage(LOG_INFO, "Enabled instance extension: [%s]\n",instance->extensions[i]);
		    found = true;
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
	
	if ((pthread_join(layerThread, NULL)) != 0) {
	    logMessage(LOG_ERROR, "Failed to join layer thread\n");
	    vulkan->result = VK_ERROR_EXTENSION_NOT_PRESENT;
	    goto clean;
	}
	joined = true;
	
	if (layerTask.taskRes != VK_SUCCESS) {
	    logMessage(LOG_ERROR, "Failed to enable layer [%d]\n", layerTask.taskRes);
	    vulkan->result = layerTask.taskRes;
	    goto clean;
	}
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
	if (!joined) pthread_join(layerThread, NULL);
	free(extensionProperties);
	return vulkan->result;
    }
#endif

#endif
