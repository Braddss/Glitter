#ifndef GL_LIGHT
#define GL_LIGHT
#pragma once
#include <glm/glm.hpp>

using namespace glm;

class GL_Light 
{
public:
	GL_Light(vec3 position = vec3(0, 0, 0), vec3 color = vec3(1, 1, 1), float intensity = 1) :position(position), color(color), intensity(intensity) {};

	vec3 position;
	vec3 color;
	float intensity;

private:
};


#endif
