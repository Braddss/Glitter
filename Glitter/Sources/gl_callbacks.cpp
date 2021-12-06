#include "gl_callbacks.hpp"
#include "metaDataManager.hpp"




void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{

	
	mWidth = width;
	mHeight = height;
	glViewport(0, 0, width, height);
}
