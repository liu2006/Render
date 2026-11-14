#ifndef CORE_H
#define CORE_H

#include "renderer.h"

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

#endif 
