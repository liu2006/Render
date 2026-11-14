#include "common.h"

#define VULKAN_IMPLEMENTATION
#define GLFW_IMPLEMENTATION
#define INSTANCE_IMPLEMENTATION
#define PHYSICAL_DEVICE_IMPLEMENTATION
#define LOGICAL_DEVICE_IMPLEMENTATION
#define SWAPCHAIN_IMPLEMENTATION
#define PIPELINE_IMPLEMENTATION
#define CONFIG_IMPLEMENTATION
#define VERTEX_BUFFER_IMPLEMENTATION

int main(int argc, const char **argv)
{
    Glfw glfw = { .width = width, .height = height, .rate = rate};
    Vulkan vulkan = {0};
    Instance instance = {0};
    PhysicalDevice physicalDevice = {0};
    LogicalDevice logicalDevice = {0};
    SwapChain swapChain = {0};
    Pipeline pipeline = {0};

    Renderer renderer = {
	.glfw = &glfw,
	.vulkan = &vulkan,
	.instance = &instance,
	.physicalDevice = &physicalDevice,
	.logicalDevice = &logicalDevice,
	.swapChain = &swapChain,
	.pipeline = &pipeline
    };
    
    if (!run(argc, argv, &renderer)) return 1;
    
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
}
