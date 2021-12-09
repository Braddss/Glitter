#ifndef GL_OBJECT
#define GL_OBJECT
#pragma once

#include "shader.hpp";
#include <glm/matrix.hpp>
#include <vector>

using namespace Mirage;
using namespace glm;



class GL_Object
{
public:
	Shader* shader;

	bool eboInUse;
	uint VAO;
	uint VBO;

	
	uint EBO;

	

	uint modelLoc;
	uint rotationLoc;
	uint viewLoc;
	uint projectionLoc;
	uint lightPosLoc;
	uint lightColorLoc;
	uint lightIntensityLoc;


	

	GL_Object();

	//GL_Object(float vertices[]);

	void draw();

	void translateObj(vec3 vector);
	void rotateObj(vec3 vector, float deg);
	void scaleObj(vec3 vector);
	

	vec3 getPosition();
	vec4 getRotation();
	vec3 getScale();
	mat4 getModelMat();
	


private:

	void init();
	Shader* initShader();
	uint initVAO();
	uint initVBO();
	uint initVBO(float vertices[]);
	uint initEBO();

	void processModelMat();
	mat4 model;
	mat4 rotationMat;
	vec3 position;
	vec4 rotation;
	vec3 scale;

	std::vector<float> vertices;
	int sizeTris;
	bool modelNeedsUpdate;
};




#endif GL_OBJECT