#include "Main.h" // see header file for where we include GLAD and GLFW

void Main::ValidateShader(const unsigned int shaderId)
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


int main(int argc, char* argv[])
{
	// This isn't platform safe, but for some reason this VS project doesn't have proper filesystem support despite
	// being c++17, and the latest boost libs are not compiling properly for me. So I'm just going to manually
	// handle paths. This is going to be used to get the vertex and fragment shader files, which I've configured
	// to be copied into the output directory in the LearnOpenGL.vsxproj file.
	std::string fullPathToExe = argv[0];
	std::string appPath = fullPathToExe.substr(0, fullPathToExe.find_last_of("\\"));
	Main g(appPath);
	int ret = g.mainImplRectangleWithEBO();
	return ret;
}

Main::Main(std::string appPath)
{
	this->m_appPath = appPath;
}

int Main::mainImplTriangleWithVBO() {
	// SETUP
	glfwInit();

	// this tells glfw that the available functions must be OpenGL 3.3 or later
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// we want to use a smaller subset of OpenGL features
	// setting to core profile will exclude a bunch of backwards compatible features for versions older than OpenGL 3.3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	GLFWwindow* window = glfwCreateWindow(m_windowWidth, m_windowHeight, "LearnOpenGL", NULL, NULL);
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
	glViewport(0, 0, m_windowWidth, m_windowHeight); // The first two params set the location of the lower left corner of the window. The last two set the upper right.
	// This is used to map the normalized device coords (-1 to 1) to screen coords (shifted to the location of your window)

	// setting the GL viewport smaller than the glfw window means we have empty space that can be used to display other elements outside of the viewport

	// set a callback for when the window is resized. This is a function we define
	// this is also called the first time window is displayed
	// * for retina displays, the actual width/height will be higher than the original input values to glViewport
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// single triangle
	/*float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
	};*/

	float vertices1[] = {
	 -0.9f, 0.2f, 0.0f,
	 -0.1f, 0.2f, 0.0f,
	 -0.5f,  0.8f, 0.0f,
	};

	float vertices2[] = {
		0.1f, -0.8f, 0.0f,
		0.9f, -0.8f, 0.0f,
		0.5f, -0.2f, 0.0f
	};

	// VBOs (Vertex Buffer Objects) are an OpenGL object that let you send large amount of data from CPU to GPU, without having to send it one vertex at a time. OpenGL objects have a unique ID, and these IDs are generated for VBOs like so:

	// one VBO per triangle
	unsigned int VBOs[2];

	// INSTANTIATE A BUFFER
	// params are:
	// number of buffers you want to generate
	// address of array of buffer IDs (unsigned ints) that will be populated
	glGenBuffers(2, VBOs);
	// since we only want one, we just give it the address of a single unsigned int

	// BIND THE BUFFER TO A BUFFER_TYPE
	// the buffer now exists, but you need to tie it to one of the buffer types
	// one and only one buffer may be assigned to each buffer type
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]); // assign the buffer with buffer ID VBO to the buffer type GL_ARRAY_BUFFER
	// from now on any buffer calls made to GL_ARRAY_BUFFER will use the instantiated buffer identified by VBO

	// WRITE THE DATA TO THE BOUND BUFFER THROUGH THE BUFFER TYPE
	// this says to write memory into the buffer bound to GL_ARRAY_BUFFER,
	// the number of bytes is specified by the 2nd param, and a pointer to the memory
	// is given as the 3rd param.
	// GL_STATIC_DRAW is a usage type, saying that we want to set this data and have it
	// be used by the GPU many times
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

	// other options are GL_DYNAMIC_DRAW, where data will change and is used by GPU many times
	// and GL_STREAM_DRAW, where it doesn't change and is used only a few times

	// orange program
	unsigned int shaderProgram = createBasicShaderProgram();
	unsigned int yellowShaderProgram = createBasicShaderProgram("1.0f, 1.5f, 0.2f, 1.0f");

	// BOOKMARK: "Linking Vertex Attributes"
	// recall: vertex attribute = data about one particular vertex
	// we only define one vertex attribute, aPos
	// but we've only given openGL an array, and we haven't told it how that array maps into a list of aPos values

	// Call glVertexAttribPointer, which will inform openGL about one of the vertex attributes, in this case aPos.
	// (Each parameter will be named and described)
	// This method will use whichever Vertex Buffer Object is currently bound to GL_ARRAY_BUFFER, which in our case is the previously declared local, VBO
	glVertexAttribPointer(
		0,
		// index: in our vertex shader we set the aPos vertex attribute to be at "layout (location 0)"
		// which means we need to tell openGL to interpret the location 0 of the vertices we pass
		// in the buffer as the aPos vertex attribute
		3,
		// size: size of the vertex attribute in terms of how many values is in it. The vector contains 3 values, so size is 3
		// note this is NOT the size in bytes. That is accounted for in the next parameters
		GL_FLOAT,
		// type: the type of the values. Each of the x, y, and z components in the vector are floats
		GL_FALSE,
		// normalized: Whether we want the values to be normalized. If instead of floats we used ints, the ints
		// would be normalized to 0 and 1 (unsigned) or -1 and 1 (signed). This is irrelevant for us, so leave as false
		3 * sizeof(float),
		// stride: The space between consecutive vertex attributes. Since all the positions are tightly packed, because
		// we have no other vertex attributes (e.g. color), the stride is just the size of a vertex in bytes. If there
		// were other vertex attributes we'd have to add their sizes to this value
		(void*)0
		// offset: Where the position data begins in the buffer. Our vector attributes start right at the first index in the array so
		// this is 0. The parameter type is a void* so we have to cast it.
		// Side note: ugh!! It's a C style cast? And into a void pointer?! Gross!!
	);

	// Vertex attributes are disabled by default. We need to enable them using their location. aPos is at location 0
	glEnableVertexAttribArray(0); // pass location 0


	// VERTEX ARRAY OBJECTS (VAO)
	// The way we've bound our buffer + configured the vertex attributes, we need to do it for every frame.
	// This is cumbersome and expensive. Instead we want to store buffer state into an object
	// and simply bind that object to some OpenGL object ID
	// VAO is the persistent version of VBO. You can refer to a VAO instead of rebinding a VBO every single frame. To change data you can just switch between VAO IDs

	// A VAO stores:
	//		calls to glEnableVertexAttribArray + glDisableVertexAttribArray (used to toggle on/off vertex attributes)
	//		calls to glVertexAttribPointer for vertex attribute configurations
	//		VBOs containing a list of all the vertex attributes being configured.
	//			These are the equivalent of the VBO we made earlier, but expanded out into a list of attributes rather than a list of vertices

	// They're basically a special buffer designed specifically for vertices

	unsigned int VAOs[2];
	glGenVertexArrays(2, VAOs);
	glBindVertexArray(VAOs[0]); // binds the VAO. Now all calls on VBO, glVertexAttribPointer, and glEnableVertexAttribArray will write to the VAO

	// rebind VBO and configure its vertex attributes. We repeat this so that it gets written into the now-bound VAO
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

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

		float timeValue = glfwGetTime();
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f; // sin oscillation between 0 and 1

		// this thing will return -1 if the uniform is optimized away or you spelt it incorrectly
		int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
		std::cout << greenValue << std::endl;

		// ACTIVATE THE PROGRAM
		glUseProgram(shaderProgram);
		// every shader and render call will now use this program object

		// bind the VAO for this frame
		glBindVertexArray(VAOs[0]);

		// actual command to draw the triangle
		glDrawArrays(
			GL_TRIANGLES, // the primitive we wish to drawy
			0, // starting index of the vertex array we'd like to draw
			sizeof(vertices1) // how many vertices we want to draw
		);

		//glUseProgram(yellowShaderProgram);
		glBindVertexArray(VAOs[1]);
		glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices2));

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

unsigned int Main::createBasicShaderProgram() {
	ShaderLoader shaderLoader;
	auto vertexPath = m_appPath + "\\vertex.glsl";
	auto fragmentPath = m_appPath + "\\fragment.glsl";
	return shaderLoader.createBasicShaderProgram(
		vertexPath.c_str(),
		fragmentPath.c_str());
}

unsigned int Main::createBasicShaderProgram(std::string fragColorString) {

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
	std::string stringFragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"    FragColor = vec4(" + std::string(fragColorString) + "); \n"
		"}\0";
	const char* fragmentShaderSource = stringFragmentShaderSource.c_str();

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

	// CLEANUP
	// once you link shader objects to programs, you don't need them anymore
	// this deallocs memory used for the shader object
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	return shaderProgram;

}

// callback for when window is resized by user. The width and height are the new dimensions
void Main::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	// tell OpenGL to update the scaling parameters used to convert from normalized screen coords to screen coords
}

void Main::processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

/// <summary>
/// Specifying every triangle means specifying duplicated vertices if you just want to draw n-gons.
/// Element Buffer Objects (EBOs) let you specify vertices in an array, then use indexes into that array
/// to specify primitives for those n-gons. Such an example of a more-complex-than-a-triangle primitive is
/// a rectangle
/// </summary>
int Main::mainImplRectangleWithEBO() {
	glfwInit(); // setup window
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // using core OpenGL
	GLFWwindow* window = glfwCreateWindow(m_windowWidth, m_windowHeight, "Rectango", NULL, NULL);
	if (window == NULL) {
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

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	
	float vertices[] = {
	//  vertex positions                    vertex colours
	 0.5f,  0.5f, 0.0f,  /*top right*/     1.0f, 0.0f, 0.0f,
	 0.5f, -0.5f, 0.0f,  /*bottom right*/  0.0f, 1.0f, 0.0f,
	-0.5f, -0.5f, 0.0f,  /*bottom left*/   0.0f, 0.0f, 1.0f,
	-0.5f,  0.5f, 0.0f,   /*top left*/      0.5f, 0.5f, 0.5f
	};
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	unsigned int shaderProgram = createBasicShaderProgram();

	// initialize a VAO
	// VAOs also store element buffers. If after binding this VAO, GL_ELEMENT_ARRAY_BUFFER is bound, 
	// and an EBO is written to, then that EBO will also be stored to this VAO
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	
	// create vertex buffer
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// create element buffer
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// the vertex buffer itself remains unchanged, we are just using the element buffer to specify primitives instead
	// so this call is similar to the one in mainImplTriangleWithVBO
	// vertex position attribute is still of size 3 (2nd param)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	// vertex color attribute is also size 3 (2nd param)
	glVertexAttribPointer(
		1, // layout pos 1
		3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
		(void*)(3 * sizeof(float))); // note that the starting position is not 0, since we account for the first vertex
	glEnableVertexAttribArray(0); // enable the vertex attribute at location 0
	glEnableVertexAttribArray(1); // enable the vertex attribute at location 1

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		glClearColor(0.3f, 0.6f, 0.1f, 1.0f); // set color used when clearing
		glClear(GL_COLOR_BUFFER_BIT); // clear

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO); // bind the VAO that points to the EBO to use its vertex attribute config

		// make it draw a wireframe (can revert with GL_FILL instead of GL_LINE afterwards)
		glPolygonMode(
			GL_FRONT_AND_BACK, // we want to apply this polygon drawing mode to front and back of triangles
			GL_FILL // use GL_LINE to draw with lines, not filled shapes
		);

		// draw the elements in the buffer bound to GL_ELEMENT_ARRAY_BUFFER
		glDrawElements(
			GL_TRIANGLES, // the primitive we wish to draw
			sizeof(indices), // number of elements in the EBO we want to consider when drawing
							 // the number of triangles drawn will be this number divided by 3
			GL_UNSIGNED_INT, // the type of each element in indices, now living in an element buffer
			0 // index in indices to start reading from when drawing
		);

		glfwPollEvents();
		glfwSwapBuffers(window);

	}

	glfwTerminate();
	return 0;
}
