#pragma once

// Include GLAD to load OpenGL functions, and GLFW for basic OpenGL window/context/keyboard/mouse support

#include <glad/glad.h>		// the GLAD header file includes OpenGL headers (e.g. GL/gl.h), which are required by libraries like GLFW
#include <GLFW/glfw3.h>		// so you have to include GLAD before GLFW





class GettingStarted
{
public:
	int mainImplTriangleWithVBO();
	int mainImplRectangleWithEBO();
	unsigned int createBasicShaderProgram();
	// this must be static or it will not match the signature required by openGL for resize callbacks
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

	void ValidateShader(unsigned int vertexShader);
	void processInput(GLFWwindow* window);
private:
	const int m_windowWidth = 800;
	const int m_windowHeight = 600;

};


