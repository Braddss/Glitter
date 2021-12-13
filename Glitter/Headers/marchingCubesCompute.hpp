#ifndef MARCHING_CUBES_COMPUTE
#define MARCHING_CUBES_COMPUTE
#include <glm.hpp>

class MarchingCubesCompute 
{

	unsigned int ID;
	vec3 bounds;
	MarchingCubesCompute(const char* computePath);


	void dispatch();


	void use();

	
};


#endif // !MARCHING_CUBES_COMPUTE
