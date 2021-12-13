#ifndef MARCHING_CUBES
#define MARCHING_CUBES
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <TriObject.hpp>
#include <PointObject.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/matrix.hpp>
#include <glm/glm.hpp>


class MarchingCubes
{
public:

	std::vector<float> vertices;
	
	std::vector<uint8> pointsValues;
	glm::u16vec3 bounds;
	MarchingCubes();
	MarchingCubes(const char* fileName, glm::u16vec3 bounds);

	void processInput(GLFWwindow* window);
	void drawElements();

	float surfaceLevel;
	float spacing;
	TriObject surface;
	PointObject pointCloudObject;
private:
	int64 vertexIndexToArrIndex(int vertIndex, int64 x, int64 y, int64 z);
	
	void pointValuesFromDensityFunction();
	void pointValuesFromRaw(const char* fileName);
	void pointCloudFromPoints();
	void surfaceFromPoints();
	
	float densityFunction(int x, int y, int z);

	int sizeInBytes = 1;

	vec3 vertexInterpolation(vec3 one, vec3 two, float valOne, float val2);
};





#endif // !MARCHING_CUBES
