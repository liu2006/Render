#ifndef VULKAN_H
#define VULKAN_H

extern const int width;
extern const int height;
extern const int rate;
extern const char *requiredLayers[];
extern const char *requiredDeviceExtensions[];

#if defined(VULKAN_IMPLEMENTATION)
    const int width = 1920;
    const int height = 1080;
    const int rate = 240;
    
    const char *requiredLayers[] = { "VK_LAYER_KHRONOS_validation" };
    const char *requiredDeviceExtensions[] = { "VK_KHR_swapchain" };

    bool run(int argc, const char **argv, Renderer *renderer) {
	if (argc < 2) {
            logMessage(LOG_ERROR, "Usage: %s <shader-file>\n", argv[0]);
            return false;
	}
	Glfw *glfw = renderer->glfw;
	Vulkan *vulkan = renderer->vulkan;
	Instance *instance = renderer->instance;
	PhysicalDevice *physicalDevice = renderer->physicalDevice;
	SwapChain *swapChain = renderer->swapChain;
	Pipeline *pipeline = renderer->pipeline;
	
	if (!createWindow(renderer)) goto clean;
	if (createInstance(renderer) != VK_SUCCESS) {
	    goto clean;
	};
	if (pickPhysicalDevice(renderer) != VK_SUCCESS) {
	    goto clean;
	}
	if (createLogicalDevice(renderer) != VK_SUCCESS) {
	    goto clean;
	}
	if (!createSwapChain(renderer)) {
	    /* result = false; */
	    goto clean;
	}
	if (!createPipeline(renderer, argv[1])) {
	    /* result = false; */
	    goto clean;
	}
	if (!createVertexBuffer(renderer)) {
	    /* result = false; */
	    goto clean;
	}
	return true; 
 clean:
	if (vkDeviceWaitIdle(vulkan->device) != VK_SUCCESS) {
	    logMessage(LOG_ERROR, "Failed to wait for device idle");
	}
	vkDestroyShaderModule(vulkan->device, vulkan->shaderModule, NULL);
	free(pipeline->spirvBuffer);
	vkDestroyImageView(vulkan->device, vulkan->msaaImageView, NULL);
	vkDestroyImage(vulkan->device, vulkan->msaaImage, NULL);
	vkFreeMemory(vulkan->device, vulkan->msaaImageMemory, NULL);
	for (uint32_t i = 0; i < swapChain->imageCount; ++i) {
	    vkDestroyImageView(vulkan->device, vulkan->imageViews[i], NULL);
	}
	free(vulkan->imageViews);
	free(vulkan->images);
	vkDestroySwapchainKHR(vulkan->device, vulkan->swapChain, NULL);
	vkDestroyDevice(vulkan->device, NULL);
	vkDestroySurfaceKHR(vulkan->instance, vulkan->surface, NULL);
	for (uint32_t i = 0; i < physicalDevice->deviceExtensionCount; ++i) {
	    free((void *)physicalDevice->deviceExtensions[i]);
	}
	vkDestroyInstance(vulkan->instance, NULL);
	for (uint32_t i = 0; i < instance->layerCount; ++i) {
	    free((void *)instance->layers[i]);
	}
	free(instance->layers);
	if (glfw->initialized) {
	    if (glfw->window) {
		glfwDestroyWindow(glfw->window);
	    }
	    glfwTerminate();
	}
	return false;
    }
    
    void logMessage(LogLevel level, const char *format, ...)
    {
	FILE *stream;
	if (level == LOG_NONE) return;

	stream = (level == LOG_ERROR) ? stderr : stdout;
	if (level < LOG_INFO || level > LOG_ERROR)
	    return;

	const char *levelName[] = {
	    [LOG_INFO] = "INFO",
	    [LOG_WARNING] = "WARNING",
	    [LOG_ERROR] = "ERROR"
	};

	fprintf(stream, "[%s] ", levelName[level]);
	va_list args;
	va_start(args, format);
	vfprintf(stream, format, args);
	va_end(args);
	fflush(stream);
    } 
#endif
    
#endif

