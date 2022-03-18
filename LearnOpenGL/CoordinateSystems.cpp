#include "CoordinateSystems.h"


CoordinateSystems::CoordinateSystems(IApplicationParamsProvider* appParamsProvider) : Texturing(appParamsProvider) {
}

int CoordinateSystems::ExecuteWindow(GLFWwindow* window, Shader& shader, Shader& shader2, unsigned int VAO, unsigned int VAO2, unsigned int texture1, unsigned int texture2)
{

    
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_DEPTH_TEST); // z is important. It doesn't check z buffer by default.
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        m_deltaTime = currentFrame - m_lastFrame;
        m_lastFrame = currentFrame;
        GLFWUtilities::closeWindowIfEscapePressed(window);
        CoordinateSystems::processInput(window);
        glClearColor(0.3f, 0.6f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear both the color and z buffers, or the previous frame's z will be there.

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        shader.use();

        // MODEL MATRIX
        //glm::mat4 model = glm::mat4(1.0);
        //model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        //model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

        // VIEW MATRIX
        //const float radius = 10.0f; // set the camera position to circle around the origin
        //float camX = sin(glfwGetTime()) * radius;
        //float camZ = cos(glfwGetTime()) * radius;

        glm::mat4 view;
        //view = glm::lookAt(
        //    glm::vec3(camX, 0.0, camZ), // camera position
        //    glm::vec3(0.0, 0.0, 0.0), // origin (target to look at)
        //    glm::vec3(0.0, 1.0, 0.0)); // world up vector (y axis is up

        /*view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        view = glm::rotate(view, (float)glfwGetTime() * glm::radians(2.0f), glm::vec3(0.5f, 1.0f, 0.0f));*/

        view = glm::lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);

        // PERSPECTIVE PROJECTION MATRIX
        glm::mat4 projection;

        // fov, aspect ratio, near, far
        projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

        //shader.setMat4("model", glm::value_ptr(model));
        shader.setMat4("view", glm::value_ptr(view));
        shader.setMat4("projection", glm::value_ptr(projection));
        
        glBindVertexArray(VAO);
        for (unsigned int i = 0; i < 10; ++i)
        {
            // translate the cube to its position in cubePositions[i], and rotate it about <1, 0.3, 0.5> axis 20 degrees times i (+ a bit more per unit time passed, for every third box)
            // this will be used as the model matrix in the vertex shader
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            if (i % 3 == 0) {
                angle += (float)glfwGetTime() * 5.0f;
            }
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            shader.setMat4("model", glm::value_ptr(model));

            glDrawArrays(GL_TRIANGLES, 0, 36);

        }

        //glDrawElements(GL_TRIANGLES, sizeof(m_indices), GL_UNSIGNED_INT, 0);

        glfwPollEvents();

        glfwSwapBuffers(window);
    }
    glfwTerminate();
    return 0;
}

const float* CoordinateSystems::GetVertices(size_t& size)
{
    size = sizeof(m_verticesCube);
    return m_verticesCube;
}

const char* CoordinateSystems::GetVertexShaderPath()
{
    return m_overriddenVertexShader;
}

const char* CoordinateSystems::GetFragmentShaderPath()
{
    return m_overriddenFragmentShader;
}

void CoordinateSystems::CreateRectangle(GLuint& VAO)
{
    // array object to store raw vertices, element buffers, and vertex attribute settings
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // raw vertices
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    size_t size;
    const float* vertices = GetVertices(size);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

    // element buffer
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_indices), m_indices, GL_STATIC_DRAW);

    VertexBufferLayout vertexBufferLayout(std::vector<int>{3, 2});
    vertexBufferLayout.Process();
    //GLsizei stride = 5 * sizeof(float);
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    //glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    //glEnableVertexAttribArray(0);
    //glEnableVertexAttribArray(1);
}

void CoordinateSystems::processInput(GLFWwindow *window) {
    float cameraSpeed = 2.5f * m_deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        m_cameraPos += cameraSpeed * m_cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        m_cameraPos -= cameraSpeed * m_cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        m_cameraPos -= glm::normalize(glm::cross(m_cameraFront, m_cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        m_cameraPos += glm::normalize(glm::cross(m_cameraFront, m_cameraUp)) * cameraSpeed;

}
