#include "OpenGLUtilities.h"

// callback for when window is resized by user. The width and height are the new dimensions
void OpenGLUtilities::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}