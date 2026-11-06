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

bool createSwapChain(Vulkan *vulkan, PhysicalDevice *physicalDevice, SwapChain *swapChain, Glfw *glfw);

#if defined(SWAPCHAIN_IMPLEMENTATION)
bool createSwapChain(Vulkan *vulkan, PhysicalDevice *physicalDevice, SwapChain *swapChain, Glfw *glfw) 
{
    vulkan->result = glfwCreateWindowSurface(vulkan->instance, glfw->window, NULL, &vulkan->surface);
    if (vulkan->result != VK_SUCCESS) {
	logMessage(LOG_ERROR, "Failed to create window surface [%d]\n", vulkan->result);
	goto clean;
    }
    VkSurfaceCapabilitiesKHR surfaceCapabilities;
    vulkan->result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vulkan->physicalDevice, 
							       vulkan->surface, 
							       &surfaceCapabilities);
    if (vulkan->result != VK_SUCCESS) {
	logMessage(LOG_ERROR, "Failed to get surface capabilities [%d]\n", vulkan->result);
	goto clean;
    } 
    int width, height;
    glfwGetFramebufferSize(glfw->window, &width, &height);
    swapChain->surfaceExtent.width = width;
    swapChain->surfaceExtent.height = height;
    if (surfaceCapabilities.currentExtent.width != UINT32_MAX) {
	swapChain->surfaceExtent = surfaceCapabilities.currentExtent;
    }
    if (surfaceCapabilities.minImageCount >= 2) {
	swapChain->imageCount = 2;
    }
    vulkan->result = vkGetPhysicalDeviceSurfaceFormatsKHR(vulkan->physicalDevice, 
							  vulkan->surface, &swapChain->formatCount, NULL);
    if (vulkan->result != VK_SUCCESS) {
	logMessage(LOG_ERROR, "Failed to get surface formats [%d]\n", vulkan->result);
	goto clean;
    } 
    swapChain->formats = malloc(sizeof(*swapChain->formats) * swapChain->formatCount);
    vulkan->result = vkGetPhysicalDeviceSurfaceFormatsKHR(vulkan->physicalDevice, 
							  vulkan->surface, &swapChain->formatCount, 
							  swapChain->formats);
    if (vulkan->result != VK_SUCCESS) {
	logMessage(LOG_ERROR, "Failed to get surface formats [%d]\n", vulkan->result);
	goto clean;
    } 
    vulkan->result = vkGetPhysicalDeviceSurfacePresentModesKHR(vulkan->physicalDevice, 
							       vulkan->surface, 
							       &swapChain->presentModeCount,
							       NULL);
    if (vulkan->result != VK_SUCCESS) {
	logMessage(LOG_ERROR, "Failed to get surface present modes [%d]\n", vulkan->result);
	goto clean;
    }     
    swapChain->presentModes = malloc(sizeof(*swapChain->presentModes) * swapChain->presentModeCount);
    vulkan->result = vkGetPhysicalDeviceSurfacePresentModesKHR(vulkan->physicalDevice, 
							       vulkan->surface, 
							       &swapChain->presentModeCount,
							       swapChain->presentModes);
    if (vulkan->result != VK_SUCCESS) {
	logMessage(LOG_ERROR, "Failed to get surface present modes [%d]\n", vulkan->result);
	goto clean;
    }     
    for (uint32_t i = 0; i < swapChain->formatCount; ++i) {
	if (swapChain->formats[i].format == VK_FORMAT_B8G8R8A8_SRGB &&
	    swapChain->formats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
	    swapChain->formatIndex = i;
	    break;
	}
    }
    for (uint32_t i = 0; i < swapChain->presentModeCount; ++i) {
	if (swapChain->presentModes[i] == VK_PRESENT_MODE_FIFO_KHR) {
	    swapChain->presentModeIndex = i;
	    break;
	} 
    }
    const VkSwapchainCreateInfoKHR swapChainInfo = {
	.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
	.pNext = NULL,
	.flags = 0,
	.surface = vulkan->surface,
	.minImageCount = swapChain->imageCount,
	.imageFormat = swapChain->formats[swapChain->formatIndex].format,
	.imageColorSpace = swapChain->formats[swapChain->formatIndex].colorSpace,
	.imageExtent = swapChain->surfaceExtent,
	.imageArrayLayers = 1,
	.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
	.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
	.queueFamilyIndexCount = 1,
	.pQueueFamilyIndices = NULL,
	.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
	.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
	.presentMode = swapChain->presentModes[swapChain->presentModeIndex],
	.clipped = VK_TRUE,
	.oldSwapchain = VK_NULL_HANDLE,
    };
    vulkan->result = vkCreateSwapchainKHR(vulkan->device, &swapChainInfo, NULL, &vulkan->swapChain);
    if (vulkan->result != VK_SUCCESS) {
	logMessage(LOG_ERROR, "Failed to create swap chain [%d]\n", vulkan->result);
	goto clean;
    }     
    logMessage(LOG_INFO, "Swap chain created\n", vulkan->result);
    vulkan->images = malloc(sizeof(*vulkan->images) * swapChain->imageCount);
    vulkan->result = vkGetSwapchainImagesKHR(vulkan->device, vulkan->swapChain, &swapChain->imageCount, vulkan->images);
    if (vulkan->result != VK_SUCCESS) {
	logMessage(LOG_ERROR, "Failed to get swap chain images [%d]\n", vulkan->result);
	goto clean;
    }     
    VkImageViewCreateInfo imageViewInfo = { 
        .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
	.pNext = NULL,
	.flags = 0,
	.image = VK_NULL_HANDLE,
	.viewType = VK_IMAGE_VIEW_TYPE_2D,
	.format = swapChain->formats[swapChain->formatIndex].format,
	.components = {
	    .r = VK_COMPONENT_SWIZZLE_IDENTITY,
	    .g = VK_COMPONENT_SWIZZLE_IDENTITY,
	    .b = VK_COMPONENT_SWIZZLE_IDENTITY,
	    .a = VK_COMPONENT_SWIZZLE_IDENTITY
	},
	.subresourceRange = { 
	    .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
	    .baseMipLevel = 0,
	    .levelCount = 1,
	    .baseArrayLayer = 0,
	    .layerCount = 1
	}
    };
    vulkan->imageViews = malloc(sizeof(*vulkan->imageViews) * swapChain->imageCount);
    for (uint32_t i = 0; i < swapChain->imageCount; ++i) {
	imageViewInfo.image = vulkan->images[i]; 
	vulkan->result = vkCreateImageView(vulkan->device, &imageViewInfo, NULL, &vulkan->imageViews[i]);
	if (vulkan->result != VK_SUCCESS) {
	    logMessage(LOG_ERROR, "Failed to create image view [%d]\n", vulkan->result); 
	    goto clean;
	}
    }

    const VkImageCreateInfo msaaImageInfo = {
	.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
	.pNext = NULL,
	.flags = 0,
	.imageType = VK_IMAGE_TYPE_2D,
	.format = swapChain->formats[swapChain->formatIndex].format,
	.extent = { 
	    .width = swapChain->surfaceExtent.width, 
	    .height = swapChain->surfaceExtent.height,
	    .depth = 1
	},
	.mipLevels = 1,
	.arrayLayers = 1,
	.samples = physicalDevice->msaaSamples,
	.tiling = VK_IMAGE_TILING_OPTIMAL,
	.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT,
	.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
	.queueFamilyIndexCount = 0,
	.pQueueFamilyIndices = NULL,
	.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
    };
    (void)msaaImageInfo; 
    vulkan->result = VK_SUCCESS;
    
clean:
    free(swapChain->presentModes);
    free(swapChain->formats);
    return (vulkan->result == VK_SUCCESS) ? true : false;
} 
#endif

#endif
