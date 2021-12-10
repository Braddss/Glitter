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

   // MarchingCubes mCubeObject;

   /* glm::u32vec3 bounds(98, 34, 34);
    MarchingCubes mCubeObject("silicium_98x34x34_uint8.raw", bounds);*/

    

    /*    glm::u32vec3 bounds(301, 324, 56);
    MarchingCubes mCubeObject("lobster_301x324x56_uint8.raw", bounds);*/
    glm::u32vec3 bounds(256, 256, 178);
    MarchingCubes mCubeObject("boston_teapot_256x256x178_uint8.raw", bounds);

   /* glm::u32vec3 bounds(256,256,256);
    MarchingCubes mCubeObject("bonsai_256x256x256_uint8.raw", bounds);*/
   
    

    LineObject line(std::vector<float>(coordinateSystem, coordinateSystem + sizeof coordinateSystem / sizeof coordinateSystem[0]));



    float counter = 0;
   

    mCubeObject.pointCloudObject.translateObj(vec3(1, 1, 1));
    mCubeObject.surface.translateObj(vec3(1, 1, 1));


   
    // 
    //__________________________________________________________
    // Rendering Loop
    while (glfwWindowShouldClose(mWindow) == false) {
     
        currentTime = glfwGetTime();
        deltaTime = currentTime - lastFrame;
        lastFrame = currentTime;
     

        processInput(mWindow);
        mCubeObject.processInput(mWindow);
        // Background Fill Color
        glClearColor(0.0f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);


        light.position = camera.Position;
       // box.draw();
        line.draw();
        //box3.draw();
        mCubeObject.drawElements();
        glBindVertexArray(0);
        // Flip Buffers and Draw
        glfwSwapBuffers(mWindow);
        glfwPollEvents();

        counter += 0.03f;
        if (counter > 2)counter = 0;
    }   
    
    glfwTerminate();
    return EXIT_SUCCESS;
}



