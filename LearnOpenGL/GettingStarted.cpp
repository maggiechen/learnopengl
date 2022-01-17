#include "GettingStarted.h" // see header file for where we include GLAD and GLFW
#include <iostream>

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