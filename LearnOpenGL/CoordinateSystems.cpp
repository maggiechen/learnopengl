#include "CoordinateSystems.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

int CoordinateSystems::ExecuteWindow(GLFWwindow* window, Shader& shader, Shader& shader2, unsigned int VAO, unsigned int VAO2, unsigned int texture1, unsigned int texture2)
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    while (!glfwWindowShouldClose(window))
    {
        GLFWUtilities::closeWindowIfEscapePressed(window);

        glClearColor(0.3f, 0.6f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        shader.use();

        glm::mat4 transform = glm::mat4(1.0);
        shader.setMat4("transform", glm::value_ptr(transform));

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, sizeof(m_indices), GL_UNSIGNED_INT, 0);

        glfwPollEvents();

        glfwSwapBuffers(window);
    }
    glfwTerminate();
    return 0;
}

const float* CoordinateSystems::GetVertices(size_t& size)
{
    size = sizeof(m_verticesNormal);
    return m_verticesNormal;
}

CoordinateSystems::CoordinateSystems(IApplicationParamsProvider* appParamsProvider): Texturing(appParamsProvider) {
}


