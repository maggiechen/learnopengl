#include "Texturing.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Texturing::Texturing(IApplicationParamsProvider* appParamsProvider)
{
    m_appParamsProvider = appParamsProvider;
}

const char* Texturing::GetVertexShaderPath()
{
    return m_vertexShaderPath;
}

const char* Texturing::GetFragmentShaderPath()
{
    return m_fragmentShaderPath;
}

int Texturing::Run()
{
    GLFWwindow* window;
    int windowResult = SetupWindow(window);
    if (windowResult != 0)
    {
        return windowResult;
    }
    
    // images are defined w/ 0 along the y axis at the top, but 
    // OpenGL expects 0 to be at the bottom of the image. So we need to tell STB we want
    // images to be loaded "flipped":
    stbi_set_flip_vertically_on_load(true);

    unsigned int texture1, texture2;
    CreateTexture("container.jpg", GL_RGB, texture1, GL_CLAMP_TO_EDGE);
    CreateTexture("awesomeface.png", GL_RGBA, texture2, GL_REPEAT);
    unsigned int VAO;
    CreateRectangle(VAO);

    unsigned int VAO2;
    CreateRectangle(VAO2);

    // auto vertPath = m_appParamsProvider->GetAppPath() + "\\vertex_textured.glsl";
    auto vertPath = m_appParamsProvider->GetAppPath() + GetVertexShaderPath();
    auto fragPath = m_appParamsProvider->GetAppPath() + GetFragmentShaderPath();
    Shader shader = *(new Shader(vertPath.c_str(), fragPath.c_str()));

    // This is how you would set texture uniform, but if your shader uses only one,
    // it just sets it by default when you bind the texture. You don't even need to
    // mention the uniform's name. The location of a texture is also called a "texture unit"
    // The default texture unit is 0, so the default active texture doesn't have to assign a location
    // (Not all GPUs support this, though)
    
    // since we have two texture, we will have to set the uniforms for them
    // NOTE: the numbers used in the 2nd param should match the ones you will call later on with
    // "glActiveTexture(GL_TEXTURE<number>);", and NOT the textureIDs you get from the CreateTexture calls.
    // This is essentially saying to the pipeline, when the shaders ask for these texture sampler uniforms,
    // grab them from the available Textures 0 and 1 (OpenGL supports up to 16). It is the glBindTexture
    // call in the render loop that attaches the actual textureIDs to these "active" textures.
    shader.use(); // VERY IMPORTANT: you must USE the program before you can set any uniforms, or it will not do anything!!
    shader.setInt("texture1", 0);
    shader.setInt("texture2", 1);
    shader.setFloat("interp", m_interp);

    Shader shader2 = *(new Shader(vertPath.c_str(), fragPath.c_str()));
    shader2.use();
    shader2.setInt("texture1", 0);
    shader2.setInt("texture2", 1);
    shader2.setFloat("interp", m_interp);

    return ExecuteWindow(window, shader, shader2, VAO, VAO2, texture1, texture2);
}

void Texturing::GetTransform(glm::mat4& transform) {
    transform = glm::translate(transform, glm::vec3(0.5, -0.5, 0.5));
    transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0, 0, 1));
}

void Texturing::GetTransform2(glm::mat4& transform) {
    transform = glm::translate(transform, glm::vec3(-0.5, 0.5, 0.5));
    float scaleScalar = 0.5f * sin((float)glfwGetTime()) + 0.5f;
    std::cout << scaleScalar << std::endl;
    transform = glm::scale(transform, glm::vec3(scaleScalar, scaleScalar, scaleScalar));
}

void Texturing::CreateTexture(std::string imageFileName, GLenum format, GLuint& textureID, GLint wrapMode)
{
    int width, height, colorChannelCount;

    // stbi = STB_image. STB = Sean T. Barrett, author of the library
    // image is a bunch of 8 bit (char) values. 6 for rgb, 2 for alpha
    unsigned char* data = stbi_load((m_appParamsProvider->GetAppPath() + "\\" + imageFileName).c_str(), &width, &height, &colorChannelCount, 0);
    // *Skimming the stb implementation, I think the last parameter is for specifying how
    // many channels you want to retrieve. Setting it to 0 just returns all 4 channels

    // create a texture in OpenGL's state, bind it to GL_TEXTURE_2D
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    // actually generate the texture from the loaded container.jpg data
    glTexImage2D(
        // specifies to generate a texture on the currently bound texture of target GL_TEXTURE_2D
        // any textures bound to GL_TEXTURE_1D or _3D are unaffected
        GL_TEXTURE_2D,
        // mipmap level to create texture for (first lvl is 0)
        0,
        // format to store texture in
        GL_RGB,
        width,
        height,
        // this param should always be 0. It's a legacy thing
        0,
        // source format
        format,
        // source per-pixel type
        GL_UNSIGNED_BYTE,
        // finally, the image data itself
        data
    );
    // create multiple levels of mipmaps for the texture
    // OpenGL just has a function for this which is pretty convenient
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data); // good practice to clean shit up!
}

void Texturing::CreateRectangle(GLuint& VAO)
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

    GLsizei stride = 8 * sizeof(float);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
}

int Texturing::SetupWindow(GLFWwindow*& window)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(m_windowWidth, m_windowHeight, "Texturing", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, m_windowWidth, m_windowHeight);

    glfwSetFramebufferSizeCallback(window, OpenGLUtilities::framebuffer_size_callback);
    return 0;
}

int Texturing::ExecuteWindow(GLFWwindow* window, Shader& shader, Shader& shader2, unsigned int VAO, unsigned int VAO2, unsigned int texture1, unsigned int texture2) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    while (!glfwWindowShouldClose(window))
    {
        GLFWUtilities::closeWindowIfEscapePressed(window);
        updateInterpAmount(window, shader);

        glClearColor(0.3f, 0.6f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        shader.use();

        glm::mat4 transform = glm::mat4(1.0);
        GetTransform(transform);
        shader.setMat4("transform", glm::value_ptr(transform));

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, sizeof(m_indices), GL_UNSIGNED_INT, 0);

        shader2.use();
        glm::mat4 transform2 = glm::mat4(1.0);
        GetTransform2(transform2);
        shader2.setMat4("transform", glm::value_ptr(transform2));
        glBindVertexArray(VAO2);
        glDrawElements(GL_TRIANGLES, sizeof(m_indices), GL_UNSIGNED_INT, 0);

        glfwPollEvents();

        glfwSwapBuffers(window);
    }
    glfwTerminate();
    return 0;
}

/// <summary>
/// The shader program clamps the value already but I want to do it here too
/// so that I don't have to wait a long time if I pressed up/down for too long before the
/// image changes
/// </summary>
void Texturing::updateInterpAmount(GLFWwindow* window, Shader& shader)
{
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        m_interp -= m_fadeSpeed;
        m_interp = std::clamp(m_interp, 0.f, 1.f);
        shader.setFloat("interp", m_interp);
    }
    else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        m_interp += m_fadeSpeed;
        shader.setFloat("interp", m_interp);
        m_interp = std::clamp(m_interp, 0.f, 1.f);
    }
}

const float* Texturing::GetVertices(size_t& size) {
    size = sizeof(m_vertices);
    return m_vertices;
}