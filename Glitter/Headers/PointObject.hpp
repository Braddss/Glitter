#ifndef POINT_OBJECT
#define POINT_OBJECT

#pragma once
#include "gl_object.hpp"

class PointObject :virtual public GL_Object
{
public:
	PointObject() : GL_Object()
	{
		shader = this->initShader();
		VAO = this->initVAO();
		VBO = this->initVBO();
	};
	
	PointObject(std::vector<float> vertices) : GL_Object(vertices)
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