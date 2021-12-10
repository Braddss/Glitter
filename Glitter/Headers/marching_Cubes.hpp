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

template <class T>
class MarchingCubes
{
public:

	std::vector<float> vertices;
	std::vector<T> points;
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
	void pointValuesFromRaw(const char* fileName);
	void pointCloudFromPoints();
	

	int sizeInBytes = 1;

};





#endif // !MARCHING_CUBES
