#include "LineObject.hpp"

ShaderH LineObject::initShader()
{
    ShaderH shaderObj("unlit.vert", "unlit.frag");

    return shaderObj;
}

uint LineObject::initVBO()
{
    unsigned int VBO;

    float* vertArr = &vertices[0];
    int size = vertices.size();

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size * sizeof(vertices[0]), vertArr, GL_STATIC_DRAW);


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    /* glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
     glEnableVertexAttribArray(1);*/

    return VBO;
}



void LineObject::draw()
{
    if (modelNeedsUpdate)
    {
        processModelMat();
    }


    mat4 modelMat = model;
    mat4 projectionMat = Camera::getCamera()->GetProjectionMatrix();
    mat4 viewMat = Camera::getCamera()->GetViewMatrix();

    shader.use();
    glBindVertexArray(VAO);
    

    shader.setMat4("projection", projectionMat);
    shader.setMat4("model", modelMat);
    shader.setMat4("view", viewMat);
    shader.setVec3("unlitColor", color);

    glDrawArrays(GL_LINES, 0, sizeTris);
}
