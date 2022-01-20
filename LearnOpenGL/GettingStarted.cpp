#include "GettingStarted.h" // see header file for where we include GLAD and GLFW
#include <iostream>

void ValidateShader(const unsigned int shaderId)
{
	int success;
	char infoLog[512];
	// gets info on a shader. 2nd param is what kind of info you want to get on the shader
	// all params after that are specific to what the 2nd param was. In this case we need
	// to pass a ref to an int so that the compile status can be stored there
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		// Each shader has an info log. To get the log of the shader, call this function
		// 2nd param is the size of the buffer we want it to write log info to.
		// 3rd param is where the full size of the info log should be stored to (optional)
		// 4th param is pointer to the buffer.
		glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
	}
}

int main()
{
	// SETUP
	glfwInit();

	// this tells glfw that the available functions must be OpenGL 3.3 or later
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// we want to use a smaller subset of OpenGL features
	// setting to core profile will exclude a bunch of backwards compatible features for versions older than OpenGL 3.3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// windows have a context attached to them. Calling glfwMakeContextCurrent sets
	// that context to the current thread
	glfwMakeContextCurrent(window);

	// glfwGetProcAddress gets the function that loads the address of the OpenGL functions, which is OS specific
	// this is passed to GLAD, which uses it to load the OpenGL functions
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// this tells OpenGL the dimensions of the rendering window. The previous usage of these
	// dimensions was just to tell GLFW to make the window that size. 
	glViewport(0, 0, 800, 600); // The first two params set the location of the lower left corner of the window. The last two set the upper right.
	// This is used to map the normalized device coords (-1 to 1) to screen coords (shifted to the location of your window)

	// setting the GL viewport smaller than the glfw window means we have empty space that can be used to display other elements outside of the viewport

	// set a callback for when the window is resized. This is a function we define
	// this is also called the first time window is displayed
	// * for retina displays, the actual width/height will be higher than the original input values to glViewport
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
	};

	// VBOs (Vertex Buffer Objects) are an OpenGL object that let you send large amount of data from CPU to GPU, without having to send it one vertex at a time. OpenGL objects have a unique ID, and these IDs are generated for VBOs like so:

	unsigned int VBO;

	// INSTANTIATE A BUFFER
	// params are:
	// number of buffers you want to generate
	// address of array of buffer IDs (unsigned ints) that will be populated
	glGenBuffers(1, &VBO);
	// since we only want one, we just give it the address of a single unsigned int

	// BIND THE BUFFER TO A BUFFER_TYPE
	// the buffer now exists, but you need to tie it to one of the buffer types
	// one and only one buffer may be assigned to each buffer type
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // assign the buffer with buffer ID VBO to the buffer type GL_ARRAY_BUFFER
	// from now on any buffer calls made to GL_ARRAY_BUFFER will use the instantiated buffer identified by VBO

	// WRITE THE DATA TO THE BOUND BUFFER THROUGH THE BUFFER TYPE
	// this says to write memory into the buffer bound to GL_ARRAY_BUFFER,
	// the number of bytes is specified by the 2nd param, and a pointer to the memory
	// is given as the 3rd param.
	// GL_STATIC_DRAW is a usage type, saying that we want to set this data and have it
	// be used by the GPU many times
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// other options are GL_DYNAMIC_DRAW, where data will change and is used by GPU many times
	// and GL_STREAM_DRAW, where it doesn't change and is used only a few times

	// SAVE VERTEX SHADER TO A STRING
	// set up a very basic vert shader that takes a vertex and returns it unchanged
	const char* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";

	// INSTANTIATE THE SHADER
	// shaders are also OpenGL objects. This function instantiates one and returns
	// its OpenGL object ID. It returns 0 if an error occurred. The parameter to the
	// function is the type of shader you want to create
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// SET THE SHADER'S CODE TO THE VERTEX SHADER STRING
	// 2nd param is the # of strings we want to pass. We only have one shader so pass 1
	// 3rd param is an array of strings holding the source code(s). We only have one.
	// Recall that using &some_pointer is the same as the type for an array of that pointer
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);

	// CHECK THAT THE SHADER COMPILES
	ValidateShader(vertexShader);

	const char* fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\0";

	// INSTANTIATE FRAGMENT SHADER
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	ValidateShader(fragmentShader);

	// CREATE A PROGRAM, WHICH IS A SPECIFICATION OF ALL THE SHADERS YOU WANT TO RUN
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader); // can only add one shader of each shader type
	glLinkProgram(shaderProgram); // this step links the attached shaders together and makes sure their inputs and outputs match. It will fail if they don't

	// VALIDATE THE PROGRAM
	// very similar to the way we validate shaders
	int success;
	char infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
	}

	// ACTIVATE THE PROGRAM
	glUseProgram(shaderProgram);
	// every shader and render call will now use this program object

	// CLEANUP
	// once you link shader objects to programs, you don't need them anymore
	// this deallocs memory used for the shader object
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// BOOKMARK: "Linking Vertex Attributes"

	// RENDER LOOP
	while (!glfwWindowShouldClose(window)) // result will be true if window was closed
	{
		processInput(window);

		// rendering commands here
		// ...
		// you have to clear the frame at the start of the render loop or the previous
		// render result will stay on screen

		// set default color that should be used when you clear a buffer
		// this is a "state-setting function" in that it sets some property of the giant
		// state object holding the preferences of OpenGL
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		// clear the color buffer specifically (there's also the depth buffer and stencil buffer)
		// this is a "state-using function" that uses a value we just set to do some action
		glClear(GL_COLOR_BUFFER_BIT);

		// check for keyboard/mouse movements, updates window state, calls any callback methods that were registered
		glfwPollEvents();

		// the color buffer is a large 2D buffer w/ colors for each pixel in GLFW's window
		// this is the front buffer as it's displayed to the user
		// there's also a back buffer that is filled out by the current render iteration
		// this call will swap the front and back buffers,
		// resulting in the new frame being shown as output on the screen
		// doing it w/ 2 buffers is better than 1 buffer bc 1 buffer results in screen tearing,
		// bc the pixels are written to from left to right, top to bottom.
		glfwSwapBuffers(window);
	}

	glfwTerminate(); // remember to clean up
	return 0;

}


// callback for when window is resized by user. The width and height are the new dimensions
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	// tell OpenGL to update the scaling parameters used to convert from normalized screen coords to screen coords
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}