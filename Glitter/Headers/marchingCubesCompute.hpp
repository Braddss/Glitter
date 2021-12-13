#ifndef MARCHING_CUBES_COMPUTE
#define MARCHING_CUBES_COMPUTE
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <string>
using namespace glm;
class MarchingCubesCompute 
{

	unsigned int ID;
	vec3 bounds;
	MarchingCubesCompute(const char* computePath);


	void dispatch();
	void checkCompileErrors(GLuint shader, std::string type);

	void use();

	
};


#endif // !MARCHING_CUBES_COMPUTE
