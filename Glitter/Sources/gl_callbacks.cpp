
#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include "gl_callbacks.hpp"




void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
