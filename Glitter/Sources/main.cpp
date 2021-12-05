// Local Headers
#include "glitter.hpp"
#include "gl_callbacks.hpp"
#include "gl_inputs.hpp"
#include "gl_initialize.hpp"
// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Standard Headers
#include <cstdio>
#include <cstdlib>


//const char* vertexShaderSource = "#version 400 core\n"
//"layout (location = 0) in vec3 aPos;\n"
//"void main()\n"
//"{\n"
//"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
//"}\0";
//
//const char* fragmentShaderSource = "#version 400 core\n"
//"out vec4 FragColor;\n"
//"void main()\n"
//"{\n"
//"    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
//"}\0";

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

 
    //callbacks
    glfwSetFramebufferSizeCallback(mWindow, framebufferSizeCallback);
    fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));


    unsigned int shaderProgram;
    unsigned int VAO;

    Init::initialize(shaderProgram, VAO);
 
    // 
    //__________________________________________________________
    // Rendering Loop
    while (glfwWindowShouldClose(mWindow) == false) {

        


        processInput(mWindow);

        // Background Fill Color
        glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // Flip Buffers and Draw
        glfwSwapBuffers(mWindow);
        glfwPollEvents();
    }   glfwTerminate();
    return EXIT_SUCCESS;
}




////_______________________________________________________________
////initialization
//
//float vertices[] = {
//-0.5f, -0.5f, 0.0f,
// 0.5f, -0.5f, 0.0f,
// 0.0f,  0.5f, 0.0f
//};
//
////Vao
//// 
//
//unsigned int VAO;
//glGenVertexArrays(1, &VAO);
//unsigned int VBO;
//glGenBuffers(1, &VBO);
//glBindVertexArray(VAO);
//
//glBindBuffer(GL_ARRAY_BUFFER, VBO);
//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
////shaderStuff
////vertShader
//unsigned int vertexShader;
//vertexShader = glCreateShader(GL_VERTEX_SHADER);
//glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
//glCompileShader(vertexShader);
//
//
//int  success;
//char infoLog[512];
//glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
//if (!success)
//{
//    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
//
//    fprintf(stderr, "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n");
//    fprintf(stderr, infoLog);
//    fprintf(stderr, "\n");
//
//
//}
////fragShader
//unsigned int fragmentShader;
//fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
//glCompileShader(fragmentShader);
//
//
//glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
//if (!success)
//{
//    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
//
//    fprintf(stderr, "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n");
//    fprintf(stderr, infoLog);
//    fprintf(stderr, "\n");
//
//
//}
////ShaderProgram
//unsigned int shaderProgram;
//shaderProgram = glCreateProgram();
//
//glAttachShader(shaderProgram, vertexShader);
//glAttachShader(shaderProgram, fragmentShader);
//glLinkProgram(shaderProgram);
//
////ErrorCheck
//glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
//if (!success) {
//    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
//    fprintf(stderr, "ERROR::SHADER::LINKING::FAILED\n");
//    fprintf(stderr, infoLog);
//    fprintf(stderr, "\n");
//}
////~End Shader Stuff
//
//glUseProgram(shaderProgram);
//
//glDeleteShader(vertexShader);
//glDeleteShader(fragmentShader);
//
//
////Linking Vertex Attributes
//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//glEnableVertexAttribArray(0);