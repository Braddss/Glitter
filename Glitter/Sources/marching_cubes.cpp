#include "marching_cubes.hpp"
#include <math.h>





template <class T>
MarchingCubes<T>::MarchingCubes(const char* fileName, glm::u16vec3 bounds):bounds(bounds)
{
	pointCloudObject = PointObject();
	spacing = 0.1f;
	surfaceLevel = 0.8f;
	pointValuesFromRaw(fileName);
	pointCloudFromPoints();
	

}

template <class T>
void MarchingCubes<T>::pointValuesFromRaw(const char* fileName) //Todo -->pointCloud from Raw and then verticesFrom zeroLevel
{
	std::string path = PROJECT_SOURCE_DIR "/Glitter/DataSets/";
	std::string valueData;
	std::ifstream rawFile;
	rawFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);


	try
	{
		rawFile.open(path + fileName,std::ios::in| std::ios::binary);

		std::stringstream rawStream;
		rawStream << rawFile.rdbuf();

		rawFile.close();
		valueData = rawStream.str();
		
	}
	catch (std::ifstream::failure& e)
	{
		std::cout << "ERROR::RAWFILE::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	long min = 100;
	long max = 0;
	std::vector<T> pointValues;

	for (int i = 0; i < valueData.length(); i++)
	{
		long val = static_cast<long>(valueData[i]);
		T valTest =static_cast<T>(val + (pow(2, 8 * sizeof(T))/2));
		pointValues.push_back(valTest);
		if (valTest < min)min = valTest;
		if (valTest > max)max = valTest;
	};

	points = pointValues;
	std::cout << min << " " << max <<std::endl;


}

template<class T>
void MarchingCubes<T>::pointCloudFromPoints()
{
	vector<float> pointCloud;

	for (int64_t z = 0; z < bounds.z; z++)
	{
		for (int64_t y = 0; y < bounds.y; y++)
		{
			for (int64_t x = 0; x < bounds.x; x++)
			{
				pointCloud.push_back(static_cast<float>(z) * spacing);
				pointCloud.push_back(static_cast<float>(y) * spacing);
				pointCloud.push_back(static_cast<float>(x) * spacing);
				int size = pow(2, 8 * sizeof(points[0]));

				float val = static_cast<float>(points[x + y * bounds.x + z * bounds.x * bounds.y]) / size;

				pointCloud.push_back(val);
				float alpha = val < surfaceLevel ? 0 : 1;
				pointCloud.push_back(alpha);
			}
		}
	}

	pointCloudObject.updateData(pointCloud);
}

template<class T>
void MarchingCubes<T>::drawElements()
{
	pointCloudObject.draw();
}



template<class T>
void MarchingCubes<T>::processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	{
		spacing *= 1.01f;
		pointCloudFromPoints();
	}

	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
	{
		spacing *= 0.99f;
		pointCloudFromPoints();
	}

	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
	{
		surfaceLevel += 0.001f;
		if (surfaceLevel < 0)surfaceLevel = 0;
		if (surfaceLevel > 1)surfaceLevel = 1;
		pointCloudFromPoints();
	}

	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
	{
		surfaceLevel -= 0.001f;
		if (surfaceLevel < 0)surfaceLevel = 0;
		if (surfaceLevel > 1)surfaceLevel = 1;
		pointCloudFromPoints();
	}
		
}
template class MarchingCubes<uint8_t>;
template class MarchingCubes<uint16_t>;