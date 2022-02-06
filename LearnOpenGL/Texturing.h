#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "GLFWUtilities.h"
#include "IApplicationParamsProvider.h"
#include "OpenGLUtilities.h"
#include "Shader.h"
class Texturing
{
private:
	static const int m_windowWidth = 400;
	static const int m_windowHeight = 600;
	IApplicationParamsProvider* m_appParamsProvider;
    float m_vertices[32] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
    };

    // elements
    unsigned int m_indices[6] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };

    unsigned int CreateTexture();
    unsigned int CreateRectangle();
    int SetupWindow(GLFWwindow*& window);
    int ExecuteWindow(GLFWwindow* window, Shader& shader, unsigned int VAO, unsigned int texture);

public:
	Texturing(IApplicationParamsProvider* appParamsProvider);
	int RunTexturing();
};

