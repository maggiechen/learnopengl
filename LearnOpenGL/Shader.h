#pragma once

#include <glad/glad.h>
#include <string>
#include <fstream> // file stream
#include <sstream> // string stream
#include <iostream> // input/output stream
class Shader
{
public:
	unsigned int ID;
	Shader(const char* vertexPath, const char* fragmentPath);
	void use();
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setFloat2(const std::string& name, float value1, float value2);
	void setFloat4(const std::string& name, float value1, float value2, float value3, float value4);
	void setMat4(const std::string& name, const GLfloat* matrix);
};

