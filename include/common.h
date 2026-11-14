#ifndef CORE_H
#define CORE_H

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

typedef enum {
    LOG_NONE = 0,
    LOG_INFO = 1,
    LOG_WARNING = 2,
    LOG_ERROR = 3
} LogLevel;

typedef struct {
    bool initialized;
    GLFWwindow *window;
    int width;
    int height;
    int rate;
} Glfw;

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

typedef struct {
    uint32_t extensionCount;
    const char **extensions;

    uint32_t layerCount;
    const char **layers;
} Instance;

typedef struct {
    Instance *instance;
    VkResult taskRes;
} LayerTask;

typedef struct {
    uint32_t graphicsIndex;

    uint32_t deviceExtensionCount;
    const char **deviceExtensions;

    VkSampleCountFlagBits msaaSamples;
} PhysicalDevice;

typedef struct {
    VkResult taskRes;
    Vulkan *vulkan;
    PhysicalDevice *physicalDevice;
} QueueTask;

typedef struct {
    void *ptr; 
} LogicalDevice;

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

typedef struct {
    Vulkan *vulkan;
    SwapChain *swapChain;
    VkResult taskRes;
} SurfaceTask;

typedef struct {
    size_t spirvSize;
    uint32_t *spirvBuffer;
} Pipeline;

typedef struct {
    float position[2];
    float color[3];
} Vertex;

typedef struct {
    Glfw *glfw;
    Vulkan *vulkan;
    Instance *instance;
    PhysicalDevice *physicalDevice;
    LogicalDevice *logicalDevice;
    SwapChain *swapChain;
    Pipeline *pipeline;
} Renderer;

void logMessage(LogLevel level, const char *format, ...);
bool run(int argc, const char **argv, Renderer *renderer);

bool createWindow(Renderer *renderer);
void errorCallback(int errorCode, const char *description);
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);

int createInstance(Renderer *renderer);

int pickPhysicalDevice(Renderer *renderer); 

int createLogicalDevice(Renderer *renderer);

bool createSwapChain(Renderer *renderer);

bool createPipeline(Renderer *renderer, const char *fname);

bool createVertexBuffer(Renderer *renderer);

#ifdef NDEBUG
#define ENABLE_VALIDATION 0
#else 
#define ENABLE_VALIDATION 1
#endif

#if defined(GLFW_IMPLEMENTATION)
    #include "glfw.h"
#endif

#if defined(INSTANCE_IMPLEMENTATION)
    #include "instance.h"
#endif

#if defined(PHYSICAL_DEVICE_IMPLEMENTATION)
    #include "physicalDevice.h"
#endif

#if defined(LOGICAL_DEVICE_IMPLEMENTATION)
    #include "logicalDevice.h"
#endif

#if defined(SWAPCHAIN_IMPLEMENTATION)
    #include "swapChain.h"
#endif

#if defined(PIPELINE_IMPLEMENTATION)
    #include "pipeline.h"
#endif

#if defined(VERTEX_BUFFER_IMPLEMENTATION)
    #include "vertexBuffer.h"
#endif


#endif 
