#ifndef GLFW_H
#define GLFW_H

#include "vulkan.h"

typedef struct {
    GLFWwindow* window;
    int width;
    int height;
    int rate;
} Glfw;

bool createWindow(Glfw* glfw);
void errorCallback(int errorCode, const char* description);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

#endif
