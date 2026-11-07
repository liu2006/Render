#define VULKAN_IMPLEMENTATION
#define GLFW_IMPLEMENTATION
#define INSTANCE_IMPLEMENTATION
#define PHYSICAL_DEVICE_IMPLEMENTATION
#define LOGICAL_DEVICE_IMPLEMENTATION
#define SWAPCHAIN_IMPLEMENTATION

#include "vulkan.h"
#include "glfw.h"
#include "instance.h"
#include "physicalDevice.h"
#include "logicalDevice.h"
#include "swapChain.h"

int main(int argc, const char** argv)
{
    (void)argc;
    (void)argv;
    bool success = true;
    Glfw glfw = { .width = WIDTH, .height = HEIGHT, .rate = RATE};
    Vulkan vulkan = {0};
    Instance instance = {0};
    PhysicalDevice physicalDevice = {0};
    LogicalDevice logicalDevice = {0};
    SwapChain swapChain = {0};

    if (!createWindow(&glfw)) return 1;
    if (!createInstance(&vulkan, &instance)) {
	success = false;
	goto clean;
    };
    if (!pickPhysicalDevice(&vulkan, &physicalDevice)) {
	success = false;
	goto clean;
    }
    if (!createLogicalDevice(&vulkan, &logicalDevice, &physicalDevice)) {
	success = false;
	goto clean;
    }
    if (!createSwapChain(&vulkan, &physicalDevice, &swapChain, &glfw)) {
	success = false;
	goto clean;
    }
    struct timespec startTime, currentTime;
    clock_gettime(CLOCK_MONOTONIC, &startTime);
    while (!glfwWindowShouldClose(glfw.window)) { 
	glfwPollEvents();
	clock_gettime(CLOCK_MONOTONIC, &currentTime);
	float time = currentTime.tv_sec - startTime.tv_sec + (currentTime.tv_nsec - startTime.tv_nsec) * 1e-9f;
	printf("\rRun Time: %.1fs", time);
	fflush(stdout);
    }
    printf("\n");
    fflush(stdout);

 clean:
    vkDestroyImage(vulkan.device, vulkan.msaaImage, NULL);
    vkFreeMemory(vulkan.device, vulkan.msaaImageMemory, NULL);
    vkDestroyImageView(vulkan.device, vulkan.msaaImageView, NULL);

    for (uint32_t i = 0; i < swapChain.imageCount; ++i) {
	vkDestroyImageView(vulkan.device, vulkan.imageViews[i], NULL);
    }
    free(vulkan.imageViews);
    free(vulkan.images);

    if (vulkan.swapChain != VK_NULL_HANDLE) {
	vkDestroySwapchainKHR(vulkan.device, vulkan.swapChain, NULL);
    }
    if (vulkan.surface != VK_NULL_HANDLE) {
	vkDestroySurfaceKHR(vulkan.instance, vulkan.surface, NULL);
    }
    if (vulkan.device != VK_NULL_HANDLE) {
	vkDestroyDevice(vulkan.device, NULL);
    }
    for (uint32_t i = 0; i < physicalDevice.deviceExtensionCount; ++i) {
	free((void *)physicalDevice.deviceExtensions[i]);
    }
    if (vulkan.instance != VK_NULL_HANDLE) {
	vkDestroyInstance(vulkan.instance, NULL);
    }
    for (uint32_t i = 0; i < instance.layerCount; ++i) {
	free((void *)instance.layers[i]);
    }
    free(instance.layers);

    glfwDestroyWindow(glfw.window);
    glfwTerminate();
    return success ? 0 : 1;
}
