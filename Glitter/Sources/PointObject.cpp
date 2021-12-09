#include "PointObject.hpp"

ShaderH PointObject::initShader()
{
    ShaderH shaderObj("unlit.vert", "unlit.frag");


    // shaderObj.attach("/Glitter/Shaders/triangle.vert").attach("/Glitter/Shaders/triangle.frag").link().activate();


    uint sProgram = shaderObj.ID;
    modelLoc = glGetUniformLocation(sProgram, "model");
    viewLoc = glGetUniformLocation(sProgram, "view");
    projectionLoc = glGetUniformLocation(sProgram, "projection");
   /* lightPosLoc = glGetUniformLocation(sProgram, "lightPos");
    lightColorLoc = glGetUniformLocation(sProgram, "lightColor");
    lightIntensityLoc = glGetUniformLocation(sProgram, "lightIntensity");
    rotationLoc = glGetUniformLocation(sProgram, "rotation");*/
    unlitColorLoc = glGetUniformLocation(sProgram, "unlitColor");
    return shaderObj;
}

uint PointObject::initVBO()
{
    unsigned int VBO;

    float* vertArr = &vertices[0];
    int size = vertices.size();

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size * sizeof(vertices[0]), vertArr, GL_STATIC_DRAW);


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);



    return VBO;
}



void PointObject::draw()
{
    if (modelNeedsUpdate)
    {
        processModelMat();
    }


    mat4 modelMat = model;
    mat4 projectionMat = Camera::getCamera()->GetProjectionMatrix();
    mat4 viewMat = Camera::getCamera()->GetViewMatrix();

    //GL_Light light = getLights();
    shader.use();
    glBindVertexArray(VAO);



    shader.setMat4("projection", projectionMat);
    shader.setMat4("model", modelMat);
    shader.setMat4("view", viewMat);
    shader.setVec3("unlitColor", color);
  /*  shader.setMat4("rotation", rotationMat);
    shader.setVec3("lightPos", light.position);
    shader.setVec3("lightColor", light.color);*/
    //shader.setFloat("lightIntensity", light.intensity);

    glDrawArrays(GL_POINTS, 0, sizeTris);
}
