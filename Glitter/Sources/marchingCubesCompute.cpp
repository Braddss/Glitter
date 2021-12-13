#include "marchingCubesCompute.hpp"
#include <string>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>

MarchingCubesCompute::MarchingCubesCompute(const char* computePath)
{
	std::string path = PROJECT_SOURCE_DIR "/Glitter/Shaders/";
	std::string computeCode;
	std::ifstream cShaderFile;

	cShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		// open files
		cShaderFile.open(path + computePath);
		std::stringstream cShaderStream;
		cShaderStream << cShaderFile.rdbuf();
		cShaderFile.close();
		computeCode = cShaderStream.str();

		
	}
	catch (std::ifstream::failure& e)
	{
		std::cout << "ERROR::COMPUTE_SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	const char* cShaderCode = computeCode.c_str();

	unsigned int compute;

	compute = glCreateShader(GL_COMPUTE_SHADER);

	glShaderSource(compute, 1, &cShaderCode, NULL);
	glCompileShader(compute);
	checkCompileErrors(compute, "COMPUTE");

	

	ID = glCreateProgram();
	glAttachShader(ID, compute);
	glLinkProgram(ID);

	uint test = 0; //limitation
	int test2 = 0;
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, ID, &test2);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, compute, &test2);


	glDeleteShader(compute);


	
}

void MarchingCubesCompute::dispatch()
{
	use();
	glDispatchCompute(bounds.x - 1, bounds.y - 1, bounds.z - 1);
}

void MarchingCubesCompute::use()
{
	glUseProgram(ID);
}

void MarchingCubesCompute::checkCompileErrors(GLuint shader, std::string type)
{
	GLint success;
	GLchar infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}