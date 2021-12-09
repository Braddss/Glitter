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
    test();
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // GL_FILL



    
    //trans = glm::rotate(trans, glm::radians(90.0f),normalize(vec3(0.0, 0.0, 1.0)));
    //trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));


  
    GL_Light light;
    light.position = vec3(1, 2, 3);
    light.intensity = 0.97f;
    light.color=vec3(64.f/255, 247.f/255, 54.f/255);
    addLight(light);
    
    PointObject box3;
    TriObject box;
   
    LineObject box2;
    
    
    /*GL_Object box4;
    GL_Object box5;*/
    //GL_Object box4(GL_Object::Rendertype::LINE);
    //GL_Object box5(GL_Object::Rendertype::POINT);

    //box2.scaleObj(vec3(10, 0.3f, 1.3f));
    box2.translateObj(vec3(0, 0, 2));
    box.translateObj(vec3(2, 0, 0));
    //box4.translateObj(vec3(-2, 0, 0));
    //box5.translateObj(vec3(0, 0, -2));
    float counter = 0;
   

   


    glEnable(GL_DEPTH_TEST);
    // 
    //__________________________________________________________
    // Rendering Loop
    while (glfwWindowShouldClose(mWindow) == false) {
     
        currentTime = glfwGetTime();
        deltaTime = currentTime - lastFrame;
        lastFrame = currentTime;
     

        processInput(mWindow);

        // Background Fill Color
        glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);


        //box.rotateObj(vec3(0, 1, 0), counter*10);
        

        //box2.scaleObj(vec3(1) * counter/50.f);
        //box3.translateObj(vec3(1) * ((counter / 20) - 5));
        light.position = camera.Position;
        box.draw();
        box2.draw();
        box3.draw();
        /*
        box4.draw();
        box5.translateObj(vec3(0, 0, -2));
        box5.draw();

        box5.translateObj(vec3(0, 3, -2));
        box5.draw();
        */
        

        glBindVertexArray(0);
        // Flip Buffers and Draw
        glfwSwapBuffers(mWindow);
        glfwPollEvents();

        counter += 0.4f;
        if (counter > 200)counter = 0;
    }   
    
    glfwTerminate();
    return EXIT_SUCCESS;
}



void test() 
{
    
}

