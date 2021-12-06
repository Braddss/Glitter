#ifndef GL_INPUTS
#define GL_INPUTS
#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "camera.hpp"
#include "glitter.hpp"




void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

#endif // GL_INPUTS