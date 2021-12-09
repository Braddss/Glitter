#ifndef LIGHTS_MANAGER
#define LIGHTS_MANAGER
#pragma once

#include "gl_light.hpp"


extern GL_Light* lights;

void addLight(GL_Light & light);

GL_Light getLights();



#endif