#pragma once

#include <glad/glad.h>

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

class Shader
{
public:
	unsigned int ID;

	Shader(const char* veretexPath, const char* fragmentPath);

	void use();
private:
	void shader_error_detection(unsigned int shader);
	void program_error_detection();
};

