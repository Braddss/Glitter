#include "lightsManager.hpp"



GL_Light* lights;

void addLight(GL_Light & light)
{
	lights = &light;
}

GL_Light getLights()
{
	return *lights;
}

