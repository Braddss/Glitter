#include "gl_inputs.hpp"
#include "metaDataManager.hpp"



float lastX = (MetaDataManager::Instance()).mWidth / 2.0f;
float lastY = (MetaDataManager::Instance()).mHeight / 2.0f;
bool firstMouse = true;

void processInput(GLFWwindow* mWindow)
{

	if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(mWindow, true);

    float speedMultiplier = 1;
    if (glfwGetKey(mWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        speedMultiplier = 2;
   
    if (glfwGetKey(mWindow, GLFW_KEY_W) == GLFW_PRESS)
        Camera::getCamera()->ProcessKeyboard(FORWARD, deltaTime, speedMultiplier);
    if (glfwGetKey(mWindow, GLFW_KEY_S) == GLFW_PRESS)
        Camera::getCamera()->ProcessKeyboard(BACKWARD, deltaTime, speedMultiplier);
    if (glfwGetKey(mWindow, GLFW_KEY_A) == GLFW_PRESS)
        Camera::getCamera()->ProcessKeyboard(LEFT, deltaTime, speedMultiplier);
    if (glfwGetKey(mWindow, GLFW_KEY_D) == GLFW_PRESS)
        Camera::getCamera()->ProcessKeyboard(RIGHT, deltaTime, speedMultiplier);
    if (glfwGetKey(mWindow, GLFW_KEY_Q) == GLFW_PRESS)
        Camera::getCamera()->ProcessKeyboard(DOWN, deltaTime, speedMultiplier);
    if (glfwGetKey(mWindow, GLFW_KEY_E) == GLFW_PRESS)
        Camera::getCamera()->ProcessKeyboard(UP, deltaTime, speedMultiplier);
}



// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    Camera::getCamera()->ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    Camera::getCamera()->ProcessMouseScroll(yoffset);
}