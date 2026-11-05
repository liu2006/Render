#ifndef SWAPCHAIN_H
#define SWAPCHAIN_H
#include "vulkan.h"
#include "glfw.h"

typedef struct {
    VkExtent2D surfaceExtent;
    uint32_t imageCount;

    uint32_t formatCount;
    uint32_t formatIndex;
    VkSurfaceFormatKHR *formats;
    
    uint32_t presentModeCount;
    uint32_t presentModeIndex;
    VkPresentModeKHR *presentModes;
} SwapChain;

bool createSwapChain(Vulkan *vulkan, SwapChain *swapChain, Glfw *glfw); 
#endif
