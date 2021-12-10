#include "gl_object.hpp"

using namespace glm;

float verticesDefaultCube[] = {
       -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
       -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
       -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

       -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
       -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
       -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

       -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
       -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
       -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
       -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
       -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
       -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

       -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
       -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
       -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

       -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
       -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
       -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};



GL_Object::GL_Object()
{
    vertices = std::vector<float>(verticesDefaultCube, verticesDefaultCube + sizeof verticesDefaultCube / sizeof verticesDefaultCube[0]);
    sizeTris = 36;
    this->init();
    //shader = initShader();
    //EBO = initEBO();
  
   
}

GL_Object::GL_Object(std::vector<float> vertices)
{
	/*if (EBO == nullptr)
		eboInUse = false;*/
    this->vertices = vertices;
    
    this->init();
  /*  this->vertices = vertices;
    sizeTris = sizeof(vertices) / 6;
    init();
	shader = initShader();
	VAO = initVAO();
	VBO = initVBO();*/
	//EBO = initEBO();
}


void GL_Object::draw()
{
    
    if (modelNeedsUpdate) 
    {
        processModelMat();
    }

  
    mat4 modelMat = model;
    mat4 projectionMat = Camera::getCamera()->GetProjectionMatrix();
    mat4 viewMat = Camera::getCamera()->GetViewMatrix();

    GL_Light light = getLights();
    shader.use();
    glBindVertexArray(VAO);
  
 
      
    shader.setMat4("projection", projectionMat);
    shader.setMat4("model", modelMat);
    shader.setMat4("view", viewMat);
       
    shader.setMat4("rotation", rotationMat);
    shader.setVec3("lightPos", light.position);
    shader.setVec3("lightColor", light.color);
    shader.setFloat("lightIntensity", light.intensity);


    glDrawArrays(GL_TRIANGLES, 0, sizeTris);

}

void GL_Object::translateObj(vec3 vector) 
{
    position = vector;
    modelNeedsUpdate = true;
}

void GL_Object::rotateObj(vec3 vector, float deg)
{
    rotation = vec4(vector, deg);
    modelNeedsUpdate = true;
}

void GL_Object::scaleObj(vec3 vector)
{
    scale = vector;

    modelNeedsUpdate = true;
}




vec3 GL_Object::getPosition()
{
    return position;
}
vec4 GL_Object::getRotation()
{
    return rotation;
}
vec3 GL_Object::getScale()
{
    return scale;
}


void GL_Object::processModelMat()
{
    model = mat4(1.0f);
    model = translate(model, position);
    vec3 rotVec = vec3(rotation);
    rotationMat = rotate(mat4(1.0f), radians(rotation.w), normalize(rotVec)); //Todo
    model = model* rotationMat;
    
    model = glm::scale(model, scale);
    modelNeedsUpdate = false;
}

void GL_Object::init()
{
    position = vec3(0, 0, 0);
    rotation = vec4(0, 1, 0,0);
    scale = vec3(1, 1, 1);
    color = vec3(1,1,1);
    model = mat4(1.0f);

    processModelMat();

 
   

}

ShaderH GL_Object::initShader()
{
	ShaderH shaderObj("triangle.vert","/triangle.frag");


   // shaderObj.attach("/Glitter/Shaders/triangle.vert").attach("/Glitter/Shaders/triangle.frag").link().activate();


    uint sProgram = shaderObj.ID;
    return shaderObj;
}

uint GL_Object::initVAO()
{
	uint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	return VAO;
}

uint GL_Object::initVBO()
{
    unsigned int VBO;

    float* vertArr = &vertices[0];
    int size = vertices.size();

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size*sizeof(vertices[0]), vertArr, GL_STATIC_DRAW);


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

   

    return VBO;
}

uint GL_Object::initVBO(float vertices[])
{
    unsigned int VBO;



    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    return VBO;
}

uint GL_Object::initEBO()
{
    unsigned int EBO;
    unsigned int indices[] = {  // note that we start from 0!
       0, 1, 3,   // first triangle
       1, 2, 3    // second triangle
    };

    //unsigned int EBO;
    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    return EBO;
}

void GL_Object::updateData(std::vector<float> newData, int stride)
{
    sizeTris = newData.size() / stride;
    vertices = newData;

   // float* vertArr = &vertices[0];
    int size = vertices.size();

    float* vertArr = new float[size];

    for (int i = 0; i < size; i++)
    {
        vertArr[i] = vertices[i];
    }


    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, size * sizeof(vertices[0]), vertArr, GL_DYNAMIC_DRAW);

    delete[] vertArr;
    //free(vertArr);

}
