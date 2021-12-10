#include "marching_cubes.hpp"
#include <math.h>

template <class T>
MarchingCubes<T>::MarchingCubes(const char* fileName, glm::u16vec3 bounds):bounds(bounds)
{
	verticesFromRaw(fileName);
}

template <class T>
void MarchingCubes<T>::verticesFromRaw(const char* fileName)
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

template class MarchingCubes<uint8_t>;
template class MarchingCubes<uint16_t>;