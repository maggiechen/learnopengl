#include "Texturing.h"
#include "StbImageEnabler.cpp"

Texturing::Texturing(IApplicationParamsProvider* appParamsProvider)
{
    m_appParamsProvider = appParamsProvider;
}

int Texturing::RunTexturing()
{
    GLFWwindow* window;
    int windowResult = SetupWindow(window);
    if (windowResult != 0)
    {
        return windowResult;
    }

    unsigned int texture = CreateTexture();
    unsigned int VAO = CreateRectangle();

    auto vertPath = m_appParamsProvider->GetAppPath() + "\\vertex_textured.glsl";
    auto fragPath = m_appParamsProvider->GetAppPath() + "\\fragment_textured.glsl";
    Shader shader = *(new Shader(vertPath.c_str(), fragPath.c_str()));

    // This is how you would set texture uniform, but if your shader uses only one,
    // it just sets it by default when you bind the texture. You don't even need to
    // mention the uniform's name. The location of a texture is also called a "texture unit"
    // The default texture unit is 0, so the default active texture doesn't have to assign a location
    // (Not all GPUs support this, though)
    //GLint texUniformLoc = glGetUniformLocation(shader.ID, "ourTexture");
    //glUniform1i(texUniformLoc, texture);

    return ExecuteWindow(window, shader, VAO, texture);
}

unsigned int Texturing::CreateTexture()
{
    int width, height, colorChannelCount;

    // stbi = STB_image. STB = Sean T. Barrett, author of the library
    // image is a bunch of 8 bit (char) values. 6 for rgb, 2 for alpha
    unsigned char* data = stbi_load((m_appParamsProvider->GetAppPath() + "\\container.jpg").c_str(), &width, &height, &colorChannelCount, 0);
    // *Skimming the stb implementation, I think the last parameter is for specifying how
    // many channels you want to retrieve. Setting it to 0 just returns all 4 channels

    // create a texture in OpenGL's state, bind it to GL_TEXTURE_2D
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

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
        GL_RGB,
        // source per-pixel type
        GL_UNSIGNED_BYTE,
        // finally, the image data itself
        data
    );
    // create multiple levels of mipmaps for the texture
    // OpenGL just has a function for this which is pretty convenient
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data); // good practice to clean shit up!
    return texture;
}

unsigned int Texturing::CreateRectangle()
{
    // array object to store raw vertices, element buffers, and vertex attribute settings
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // raw vertices
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices, GL_STATIC_DRAW);

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
    return VAO;
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

int Texturing::ExecuteWindow(GLFWwindow* window, Shader& shader, unsigned int VAO, unsigned int texture) {
    while (!glfwWindowShouldClose(window))
    {
        GLFWUtilities::closeWindowIfEscapePressed(window);
        glClearColor(0.3f, 0.6f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        shader.use();

        //glActiveTexture(GL_TEXTURE0); // set the texture unit to 0 first
        // this is only necessary on some implementations. By default, the active texture
        // is 0, so it should just work

        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(VAO);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawElements(GL_TRIANGLES, sizeof(m_indices), GL_UNSIGNED_INT, 0);
        glfwPollEvents();

        glfwSwapBuffers(window);
    }
    glfwTerminate();
    return 0;
}