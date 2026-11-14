#ifndef CONFIG_H
#define CONFIG_H

#include "core.h"

extern const int width;
extern const int height;
extern const int rate;
extern const char *requiredLayers[];
extern const char *requiredDeviceExtensions[];

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

#if defined(CONFIG_IMPLEMENTATION)
    const int width = 1920;
    const int height = 1080;
    const int rate = 240;
    
    const char *requiredLayers[] = { "VK_LAYER_KHRONOS_validation" };
    const char *requiredDeviceExtensions[] = { "VK_KHR_swapchain" };
#endif

#endif
