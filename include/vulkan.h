#ifndef VULKAN_H
#define VULKAN_H

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <stdio.h> 
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <time.h>

#define WIDTH 1920
#define HEIGHT 1080
#define RATE 165
#define LOG_BUFFER_SIZE 256

typedef struct {
    VkResult result;
    VkInstance instance;
    VkPhysicalDevice physicalDevice;
    VkDevice device;
    VkQueue graphicsQueue;
    VkSurfaceKHR surface;
    VkSwapchainKHR swapChain;
    VkImage* swapChainImages;
} Vulkan;

typedef enum {
    LOG_NONE = 0,
    LOG_INFO = 1,
    LOG_WARNING = 2,
    LOG_ERROR = 3
} LogLevel;

void logMessage(LogLevel level, const char* format, ...);
#endif

