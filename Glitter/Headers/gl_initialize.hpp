#ifndef GL_INITIALIZE
#define GL_INITIALIZE
#pragma once


class Init
{
public:

	static void initialize(unsigned int& shaderProgram, unsigned int& VAO);
private:
	static void init_vbo();
	static void init_vao(unsigned int& VAO);
	static void init_shader(unsigned int& shaderProgram);

};
#endif // GL_INITIALIZE