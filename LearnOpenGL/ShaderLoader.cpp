#include "ShaderLoader.h"

void ShaderLoader::readFile(const char* path, std::string &result)
{
	// input file stream
	std::ifstream fileStream(path, std::ios::in);

	// getline clobbers existing value of the 2nd param string, so need to store in temp variable
	std::string tempLine;
	while (std::getline(fileStream, tempLine))
	{
		result += tempLine + "\n";
	}
	fileStream.close();
}

void ShaderLoader::ValidateShader(const unsigned int shaderId)
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
		std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << shaderId << "\n" << infoLog << std::endl;
	}
}

unsigned int ShaderLoader::createBasicShaderProgram(const char* vertShaderPath, const char* fragShaderPath) {
	std::string vertexSource;
	readFile(vertShaderPath, vertexSource);
	const char* vertData = vertexSource.c_str();
	// INSTANTIATE THE SHADER
	// shaders are also OpenGL objects. This function instantiates one and returns
	// its OpenGL object ID. It returns 0 if an error occurred. The parameter to the
	// function is the type of shader you want to create
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// SET THE SHADER'S CODE TO THE VERTEX SHADER STRING
	// 2nd param is the # of strings we want to pass. We only have one shader so pass 1
	// 3rd param is an array of strings holding the source code(s). We only have one.
	// Recall that using &some_pointer is the same as the type for an array of that pointer
	glShaderSource(vertexShader, 1, &vertData, NULL);

	// CHECK THAT THE SHADER COMPILES
	ValidateShader(vertexShader);
	std::string fragmentSource;
	readFile(fragShaderPath, fragmentSource);
	const char* fragData = fragmentSource.c_str();

	// INSTANTIATE FRAGMENT SHADER
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragData, NULL);
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
