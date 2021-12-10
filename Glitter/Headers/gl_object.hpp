#ifndef GL_OBJECT
#define GL_OBJECT
#pragma once

#include "shaderH.hpp";
#include <glm/matrix.hpp>
#include <vector>
#include "camera.hpp"
#include "lightsManager.hpp"

//using namespace Mirage;
using namespace glm;

class GL_Object
{
public:

	ShaderH shader;

	bool eboInUse;
	uint VAO;
	uint VBO;

	
	uint EBO;

	vec3 color;

	std::vector<float> vertices;

	virtual void draw();
	virtual void updateData(std::vector<float> newData, int stride);

	void translateObj(vec3 vector);
	void rotateObj(vec3 vector, float deg);
	void scaleObj(vec3 vector);
	

	vec3 getPosition();
	vec4 getRotation();
	vec3 getScale();
	mat4 getModelMat();
	

protected:

	GL_Object();
	GL_Object(std::vector<float> vertices);

	void init();
	virtual ShaderH initShader();
	uint initVAO();
	virtual uint initVBO();
	uint initVBO(float vertices[]);
	uint initEBO();

	void processModelMat();
	mat4 model;
	mat4 rotationMat;
	vec3 position;
	vec4 rotation;
	vec3 scale;
	
	
	int sizeTris;
	bool modelNeedsUpdate;
};




#endif GL_OBJECT