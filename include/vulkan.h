#ifndef VULKAN_H
#define VULKAN_H

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <stdio.h> 
#include <stdlib.h>
#include <stdarg.h>
#include <pthread.h> 
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <time.h>

#define WIDTH 1920
#define HEIGHT 1080
#define RATE 165

extern const char *requiredLayers[];
extern const char *requiredDeviceExtensions[];

typedef struct {
    VkResult result;
    VkInstance instance;
    VkPhysicalDevice physicalDevice;
    VkDevice device;
    VkQueue graphicsQueue;
    VkSurfaceKHR surface;
    VkSwapchainKHR swapChain;
    VkImage *images;
    VkImageView *imageViews;
    
    VkImage msaaImage;
    VkDeviceMemory msaaImageMemory;
    VkImageView msaaImageView;

    VkShaderModule shaderModule;
} Vulkan;

typedef enum {
    LOG_NONE = 0,
    LOG_INFO = 1,
    LOG_WARNING = 2,
    LOG_ERROR = 3
} LogLevel;

void logMessage(LogLevel level, const char *format, ...);

#if defined(VULKAN_IMPLEMENTATION)
    const char *requiredLayers[] = { "VK_LAYER_KHRONOS_validation" };
    const char *requiredDeviceExtensions[] = { "VK_KHR_swapchain" };

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

