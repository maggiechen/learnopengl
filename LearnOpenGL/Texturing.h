#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// this gets us the forward declared glm::mat4 type, which we use below
#include <glm/fwd.hpp>
#include <stb/stb_image.h>
#include <iostream>
#include <algorithm>

#include "GLFWUtilities.h"
#include "IApplicationParamsProvider.h"
#include "OpenGLUtilities.h"
#include "Shader.h"

class Texturing
{
private:
	static constexpr int m_windowWidth = 800;
	static constexpr int m_windowHeight = 600;
    float m_interp = 0.5f;
    static constexpr float m_fadeSpeed = 0.01f;
	IApplicationParamsProvider* m_appParamsProvider;
protected:
    static constexpr int m_verticesSize = 32;
    static constexpr float m_vertices[m_verticesSize] = {
        // positions          // colors           // texture coords
         0.7f,  0.7f, 0.0f,   1.0f, 0.0f, 0.0f,   1.5f, 1.5f,   // top right
         0.7f, -0.7f, 0.0f,   0.0f, 1.0f, 0.0f,   1.5f, 0.5f,   // bottom right
        -0.7f, -0.7f, 0.0f,   0.0f, 0.0f, 1.0f,   0.5f, 0.5f,   // bottom left
        -0.7f,  0.7f, 0.0f,   1.0f, 1.0f, 0.0f,   0.5f, 1.5f    // top left 
    };

    // elements
    static constexpr unsigned int m_indices[6] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };

private:
    void CreateTexture(std::string imageFileName, GLenum format, GLuint& textureID, GLint wrapMode);
    void CreateRectangle(GLuint& VAO);
    int SetupWindow(GLFWwindow*& window);

    void updateInterpAmount(GLFWwindow* window, Shader& shader);
protected:
    virtual int ExecuteWindow(GLFWwindow* window, Shader& shader, Shader& shader2, unsigned int VAO, unsigned int VAO2, unsigned int texture1, unsigned int texture2);
    virtual const float* GetVertices(size_t& size);
public:
	Texturing(IApplicationParamsProvider* appParamsProvider);
	virtual int Run();
    void GetTransform(glm::mat4& transform);
    void GetTransform2(glm::mat4& transform);
};

