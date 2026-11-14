#ifndef RENDERER_H
#define RENDERER_H

#include "base.h"

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



