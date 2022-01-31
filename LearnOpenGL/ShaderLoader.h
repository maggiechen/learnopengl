#pragma once
#include <string>
#include <fstream>
#include <glad/glad.h>
#include <iostream>

class ShaderLoader
{
private:
	void readFile(const char* path, std::string &result);

	void ValidateShader(const unsigned int shaderId);

public:
	unsigned int createBasicShaderProgram(const char* vertShaderPath, const char* fragShaderPath);

};

