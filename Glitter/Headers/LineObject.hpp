#ifndef LINE_OBJECT
#define LINE_OBJECT

#pragma once
#include "gl_object.hpp"

class LineObject : public GL_Object
{
public:
	LineObject() : GL_Object()
	{
		shader = this->initShader();
		VAO = this->initVAO();
		VBO = this->initVBO();
	};


	LineObject(std::vector<float> vertices) : GL_Object(vertices)
	{
		sizeTris = vertices.size() / 6;
		shader = this->initShader();
		VAO = this->initVAO();
		VBO = this->initVBO();
	};
	void draw();

protected:

	ShaderH initShader();
	uint initVBO();


};

#endif