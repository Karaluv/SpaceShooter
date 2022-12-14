#include <vector>
#include <stdio.h>
#include <string>
#include <iostream>
#include <cstring>
#include <fstream>
 
#include <cstring>

bool loadOBJ(
	const char *path,
	std::vector<glm::vec3> &out_vertices,
	std::vector<glm::vec2> &out_uvs,
	std::vector<glm::vec3> &out_normals)
{
	std::cout << "Trying to load OBJ 3D model " << path << "..." << std::endl;

	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;

	std::ifstream file(path);

	if (!file.is_open())
	{
		throw std::runtime_error("Failed to open file " + std::string(path));
		return false;
	}
	
	char lineHeader[128];

	while (file >> lineHeader)
	{
	if (strcmp(lineHeader, "v") == 0)
		{
			glm::vec3 vertex;
			
			file >> vertex.x;
			file >> vertex.y;
			file >> vertex.z;

			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0)
		{
			glm::vec2 uv;
			
			file >> uv.x;
			file >> uv.y;
			
			uv.y = -uv.y;
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0)
		{
			glm::vec3 normal;
			
			file >> normal.x;
			file >> normal.y;
			file >> normal.z;
			
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0)
		{
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			char symb;
			
			file >> vertexIndex[0] >> symb >> uvIndex[0] >> symb >> normalIndex[0];
			file >> vertexIndex[1] >> symb >> uvIndex[1] >> symb >> normalIndex[1];
			file >> vertexIndex[2] >> symb >> uvIndex[2] >> symb >> normalIndex[2];
			
			
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
	}

	for (unsigned int i = 0; i < vertexIndices.size(); i++)
	{
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];

		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		glm::vec2 uv = temp_uvs[uvIndex - 1];
		glm::vec3 normal = temp_normals[normalIndex - 1];

		out_vertices.push_back(vertex);
		out_uvs.push_back(uv);
		out_normals.push_back(normal);
	}
	file.close();
	
	std::cout << "3D model loaded";
	
	return true;
}
