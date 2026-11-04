#ifndef INSTANCE_H
#define INSTANCE_H
#include "vulkan.h"

#ifdef NDEBUG
#define ENABLE_VALIDATION 0
#else 
#define ENABLE_VALIDATION 1
#endif

static const char *requiredLayers[] = {"VK_LAYER_KHRONOS_validation"};

typedef struct {
    uint32_t extensionCount;
    const char **extensions;

    uint32_t layerCount;
    const char **layers;
} Instance;

bool createInstance(Vulkan* vulkan, Instance* instance);

#endif
