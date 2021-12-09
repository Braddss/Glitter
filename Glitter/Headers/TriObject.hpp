#ifndef TRI_OBJECT
#define TRI_OBJECT

#pragma once
#include "gl_object.hpp"

class TriObject :virtual public GL_Object
{
public:
	TriObject(): GL_Object()
	{
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