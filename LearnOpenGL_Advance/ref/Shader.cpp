#include "Shader.h"

Shader::Shader(const char* veretexPath, const char* fragmentPath)
{
	std::string vertexCode, fragmentCode;
	std::ifstream vertexFile, fragmentFile;

	vertexFile.exceptions(std::ifstream::badbit | std::ifstream::failbit);
	fragmentFile.exceptions(std::ifstream::badbit | std::ifstream::failbit);

	try
	{
		vertexFile.open(veretexPath);
		fragmentFile.open(fragmentPath);

		std::stringstream vShader, fShader;
		vShader << vertexFile.rdbuf();
		fShader << fragmentFile.rdbuf();

		vertexFile.close();
		fragmentFile.close();

		vertexCode = vShader.str();
		fragmentCode = fShader.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESS_READ" << std::endl;
	}

	const char* vShadercCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	unsigned int vertex, fragment;

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex,1,&vShadercCode,NULL);
	glCompileShader(vertex);
	shader_error_detection(vertex);

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	shader_error_detection(fragment);

	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	program_error_detection();
}

void Shader::use()
{
	glUseProgram(ID);
}

void Shader::shader_error_detection(unsigned int shader)
{
	int success;
	char infoLog[512];

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILE_FAILED\n" << infoLog << std::endl;
	}
}

void Shader::program_error_detection()
{
	int success;
	char infoLog[512];

	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILE_FAILED\n" << infoLog << std::endl;
	}
}
