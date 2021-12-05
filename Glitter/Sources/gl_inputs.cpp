#include "gl_inputs.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>


void processInput(GLFWwindow* mWindow)
{
	if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(mWindow, true);
}