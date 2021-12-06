#ifndef GL_INITIALIZE
#define GL_INITIALIZE
#pragma once
#include "shader.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cstdio>
#include <cstdlib>

class Init
{
public:

	static void initialize(unsigned int& shaderProgram, unsigned int& VBO, unsigned int& VAO, unsigned int& EBO);
private:
	static void init_vbo(unsigned int& VBO);
	static void init_vao(unsigned int& VAO);
	static void init_ebo(unsigned int& EBO);
	static void init_shader(unsigned int& shaderProgram);

};
#endif // GL_INITIALIZE