#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <algorithm>

#include "GLFWUtilities.h"
#include "IApplicationParamsProvider.h"
#include "OpenGLUtilities.h"
#include "Shader.h"
class Texturing
{
private:
	static const int m_windowWidth = 800;
	static const int m_windowHeight = 600;
    float m_interp = 0.5f;
    const float m_fadeSpeed = 0.01f;
	IApplicationParamsProvider* m_appParamsProvider;
    float m_vertices[32] = {
        // positions          // colors           // texture coords
         0.7f,  0.7f, 0.0f,   1.0f, 0.0f, 0.0f,   1.5f, 1.5f,   // top right
         0.7f, -0.7f, 0.0f,   0.0f, 1.0f, 0.0f,   1.5f, 0.5f,   // bottom right
        -0.7f, -0.7f, 0.0f,   0.0f, 0.0f, 1.0f,   0.5f, 0.5f,   // bottom left
        -0.7f,  0.7f, 0.0f,   1.0f, 1.0f, 0.0f,   0.5f, 1.5f    // top left 
    };

    // elements
    unsigned int m_indices[6] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };

    void CreateTexture(std::string imageFileName, GLenum format, GLuint& textureID, GLint wrapMode);
    void CreateRectangle(GLuint& VAO);
    int SetupWindow(GLFWwindow*& window);
    int ExecuteWindow(GLFWwindow* window, Shader& shader, unsigned int VAO, unsigned int texture1, unsigned int texture2);

    void updateInterpAmount(GLFWwindow* window, Shader& shader);

public:
	Texturing(IApplicationParamsProvider* appParamsProvider);
	int RunTexturing();
};

