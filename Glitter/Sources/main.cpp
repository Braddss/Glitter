// Local Headers
#include "gl_callbacks.hpp"
#include "gl_inputs.hpp"
#include "gl_initialize.hpp"
#include "main.hpp"
#include "camera.hpp"
#include "glitter.hpp"
#include "metaDataManager.hpp"
#include "lightsManager.hpp"
#include "gl_object.hpp"
#include "TriObject.hpp"
#include "LineObject.hpp"
#include "PointObject.hpp"
#include "marching_Cubes.hpp"
#include "marching_cubes.hpp"
// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/matrix.hpp>



// Standard Headers
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>

using namespace glm;


float coordinateSystem[] = {
        0.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
        0.0f,  0.0f, 25.0f,  1.0f,  0.0f,  0.0f,
        0.0f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        0.0f,  25.0f, 0.0f,  0.0f,  1.0f,  0.0f,
        0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        25.0f,  0.0f, 0.0f,  0.0f,  0.0f,  1.0f,

};

int activeObject = 0;

void processInputForActiveObject(GLFWwindow* mWindow)
{
    if (glfwGetKey(mWindow, GLFW_KEY_1) == GLFW_PRESS)
        activeObject = 0;
    if (glfwGetKey(mWindow, GLFW_KEY_2) == GLFW_PRESS)
        activeObject = 1;
    if (glfwGetKey(mWindow, GLFW_KEY_3) == GLFW_PRESS)
        activeObject = 2;
    if (glfwGetKey(mWindow, GLFW_KEY_4) == GLFW_PRESS)
        activeObject = 3;
    if (glfwGetKey(mWindow, GLFW_KEY_5) == GLFW_PRESS)
        activeObject = 4;

}


int main(int argc, char * argv[]) {
    // Load GLFW and Create a Window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    auto mWindow = glfwCreateWindow(mWidth, mHeight, "OpenGL", nullptr, nullptr);

    // Check for Valid Context
    if (mWindow == nullptr) {
        fprintf(stderr, "Failed to Create OpenGL Context");
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // Create Context and Load OpenGL Functions
    glfwMakeContextCurrent(mWindow);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        fprintf(stderr,"Failed to initialize GLAD");
        return -1;
    }

    //gladLoadGL();
    glViewport(0, 0, mWidth, mHeight);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //glEnable(GL_POINT_SMOOTH);
    //glPointSize(6.0);

    Camera camera = Camera(glm::vec3(0, 0, 3));
    Camera::setCamera(camera);
    //callbacks
    glfwSetFramebufferSizeCallback(mWindow, framebufferSizeCallback);

    glfwSetCursorPosCallback(mWindow, mouse_callback);
    glfwSetScrollCallback(mWindow, scroll_callback);
    glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));

    /*int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;*/

  
    GL_Light light;
    light.position = vec3(1, 2, 3);
    light.intensity = 0.97f;
    light.color=vec3(64.f/255, 247.f/255, 54.f/255);
    addLight(light);

    MarchingCubes mCubeObject0;

    glm::u32vec3 bounds1(98, 34, 34);
    MarchingCubes mCubeObject1("silicium_98x34x34_uint8.raw", bounds1);
   

    glm::u32vec3 bounds2(301, 324, 56);
    MarchingCubes mCubeObject2("lobster_301x324x56_uint8.raw", bounds2);
    glm::u32vec3 bounds3(256, 256, 178);
    MarchingCubes mCubeObject3("boston_teapot_256x256x178_uint8.raw", bounds3);

    glm::u32vec3 bounds4(256,256,256);
    MarchingCubes mCubeObject4("bonsai_256x256x256_uint8.raw", bounds4);
   
    

    LineObject line(std::vector<float>(coordinateSystem, coordinateSystem + sizeof coordinateSystem / sizeof coordinateSystem[0]));



 
   

    mCubeObject0.pointCloudObject.translateObj(vec3(1, 1, 1));
    mCubeObject0.surface.translateObj(vec3(1, 1, 1));

    mCubeObject1.pointCloudObject.translateObj(vec3(1, 1, 1));
    mCubeObject1.surface.translateObj(vec3(1, 1, 1));

    mCubeObject2.pointCloudObject.translateObj(vec3(1, 1, 1));
    mCubeObject2.surface.translateObj(vec3(1, 1, 1));

    mCubeObject3.pointCloudObject.translateObj(vec3(1, 1, 1));
    mCubeObject3.surface.translateObj(vec3(1, 1, 1));

    mCubeObject4.pointCloudObject.translateObj(vec3(1, 1, 1));
    mCubeObject4.surface.translateObj(vec3(1, 1, 1));


   
    // 
    //__________________________________________________________
    // Rendering Loop
    while (glfwWindowShouldClose(mWindow) == false) {
     
        currentTime = glfwGetTime();
        deltaTime = currentTime - lastFrame;
        lastFrame = currentTime;
     

        processInput(mWindow);
        processInputForActiveObject(mWindow);
        switch (activeObject)
        {
        case 0:
            mCubeObject0.processInput(mWindow);
            break;
        case 1:
            mCubeObject1.processInput(mWindow);
            break;
        case 2:
            mCubeObject2.processInput(mWindow);
            break;
        case 3:
            mCubeObject3.processInput(mWindow);
            break;
        case 4:
            mCubeObject4.processInput(mWindow);
            break;
        }
        
       
      
      
        // Background Fill Color
        glClearColor(0.0f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);


        light.position = camera.Position;
       // box.draw();
        line.draw();
        //box3.draw();
       
        switch (activeObject)
        {
        case 0:
            mCubeObject0.drawElements();
            break;
        case 1:
            mCubeObject1.drawElements();
            break;
        case 2:
            mCubeObject2.drawElements();
            break;
        case 3:
            mCubeObject3.drawElements();
            break;
        case 4:
            mCubeObject4.drawElements();
            break;
        }
       
        glBindVertexArray(0);
        // Flip Buffers and Draw
        glfwSwapBuffers(mWindow);
        glfwPollEvents();

       
    }   
    
    glfwTerminate();
    return EXIT_SUCCESS;
}


