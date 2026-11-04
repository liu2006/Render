#include "vulkan.h"
#include "swapChain.h"

bool createSwapChain(Vulkan *vulkan, SwapChain *swapChain, Glfw *glfw) 
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
							  vulkan->surface, &swapChain->surfaceFormatCount, NULL);
    if (vulkan->result != VK_SUCCESS) {
	logMessage(LOG_ERROR, "Failed to get surface formats [%d]\n", vulkan->result);
	goto clean;
    } 
    swapChain->surfaceFormats = malloc(sizeof(*swapChain->surfaceFormats) * swapChain->surfaceFormatCount);
    vulkan->result = vkGetPhysicalDeviceSurfaceFormatsKHR(vulkan->physicalDevice, 
							  vulkan->surface, &swapChain->surfaceFormatCount, 
							  swapChain->surfaceFormats);
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
    for (uint32_t i = 0; i < swapChain->surfaceFormatCount; ++i) {
	if (swapChain->surfaceFormats[i].format == VK_FORMAT_B8G8R8A8_SRGB &&
	    swapChain->surfaceFormats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
	    swapChain->surfaceFormatIndex = i;
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
	.imageFormat = swapChain->surfaceFormats[swapChain->surfaceFormatIndex].format,
	.imageColorSpace = swapChain->surfaceFormats[swapChain->surfaceFormatIndex].colorSpace,
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
    vulkan->swapChainImages = malloc(sizeof(*vulkan->swapChainImages) * swapChain->imageCount);
    vulkan->result = vkGetSwapchainImagesKHR(vulkan->device, vulkan->swapChain, &swapChain->imageCount, vulkan->swapChainImages);
    if (vulkan->result != VK_SUCCESS) {
	logMessage(LOG_ERROR, "Failed to get swap chain images [%d]\n", vulkan->result);
	goto clean;
    }     






    vulkan->result = VK_SUCCESS;

clean:
    free(swapChain->presentModes);
    free(swapChain->surfaceFormats);
    return (vulkan->result == VK_SUCCESS) ? true : false;
} 
