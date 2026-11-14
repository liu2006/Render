#ifndef GLFW_H
#define GLFW_H
#include "common.h"

#if defined(GLFW_IMPLEMENTATION) 
    bool createWindow(Renderer *renderer)
    {
	Glfw *glfw = renderer->glfw;

	glfwSetErrorCallback(errorCallback);
	if (!glfwInit()) {
	    logMessage(LOG_ERROR, "Failed to initialize GLFW\n");
	    return false;
	}
	glfw->initialized = true;
	logMessage(LOG_INFO, "GLFW initialized\n");
	
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_REFRESH_RATE, glfw->rate);

	glfw->window = glfwCreateWindow(glfw->width, glfw->height, "vulkan", NULL, NULL);
	if (!glfw->window) {
	    logMessage(LOG_ERROR, "Failed to create GLFW window\n");
	    glfwTerminate();
	    return false;
	}
	logMessage(LOG_INFO, "Glfw window Created\n");
	glfwSetKeyCallback(glfw->window, keyCallback);
	return true;
    }

    void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) 
    {
	(void)scancode;
	if (((key == GLFW_KEY_C && (mods & GLFW_MOD_CONTROL)) || key == GLFW_KEY_ESCAPE) && action == GLFW_PRESS) {
	    glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
    }

    void errorCallback(int errorCode, const char *description)
    {
	logMessage(LOG_ERROR, "GLFW Error [%d]: %s\n", errorCode, description);
    }
#endif
    
#endif
