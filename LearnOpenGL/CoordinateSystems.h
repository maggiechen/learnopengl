#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// this gets us the forward declared glm::mat4 type, which we use below
#include <glm/fwd.hpp>
#include <stb/stb_image.h>

#include "GLFWUtilities.h"
#include "IApplicationParamsProvider.h"
#include "OpenGLUtilities.h"
#include "Texturing.h"

class CoordinateSystems: public Texturing
{

protected:
    const float m_verticesNormal[32] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
    };
    virtual int ExecuteWindow(GLFWwindow* window, Shader& shader, Shader& shader2, unsigned int VAO, unsigned int VAO2, unsigned int texture1, unsigned int texture2);
    virtual const float* GetVertices(size_t& size);

public:
    CoordinateSystems(IApplicationParamsProvider* appParamsProvider);
};

