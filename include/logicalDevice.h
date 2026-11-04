#ifndef LOGICAL_DEVICE_H
#define LOGICAL_DEVICE_H

#include "vulkan.h"
#include "physicalDevice.h"

typedef struct {
    void *ptr; 
} LogicalDevice;

bool createLogicalDevice(Vulkan *vulkan, LogicalDevice *logicalDevice, PhysicalDevice *physicalDevice);

#endif
