#pragma once

// Include GLAD to load OpenGL functions, and GLFW for basic OpenGL window/context/keyboard/mouse support

#include <glad/glad.h>		// the GLAD header file includes OpenGL headers (e.g. GL/gl.h), which are required by libraries like GLFW
#include <GLFW/glfw3.h>		// so you have to include GLAD before GLFW


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

class GettingStarted
{
};

