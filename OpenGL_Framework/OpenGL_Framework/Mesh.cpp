#include "Mesh.h"
#include <fstream>
#include <iostream>

#define CHAR_BUFFER_SIZE 128

using namespace std;

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}

Mesh::Mesh(const std::string & file)
{
	LoadfromFile(file);
}

bool Mesh::LoadfromFile(const std::string & file)
{
	ifstream input;
	input.open(file);

	if (!input) {
		std::cout << "File open error" << std::endl;
		return false;
	}
	char inputString[CHAR_BUFFER_SIZE];

	//unique data

	//face data
	
	//unpacked data
	

	while (!input.eof()) {
		input.getline(inputString, CHAR_BUFFER_SIZE);

		if (strstr(inputString, "#") != nullptr) {
			//a comment
			continue;
		}
		else if (strstr(inputString, "o") != nullptr || strstr(inputString, "s") != nullptr || strstr(inputString, "mtllib") != nullptr || strstr(inputString, "usemtl") != nullptr) {
			//ignores unused data
			continue;
		}
		else if (strstr(inputString, "vt") != nullptr) {
			//UV data

			glm::vec2 temp;
			sscanf_s(inputString, "vt %f %f", &temp.x, &temp.y);
			textureData.push_back(temp);

		}
		else if (strstr(inputString, "vn") != nullptr) {
			//vertex data

			glm::vec3 temp;

			sscanf_s(inputString, "vn %f %f %f", &temp.x, &temp.y, &temp.z);
			normalData.push_back(temp);
		}
		else if (strstr(inputString, "v") != nullptr) {
			//vertex data

			glm::vec3 temp;
			sscanf_s(inputString, "v %f %f %f", &temp.x, &temp.y, &temp.z);
			if (temp.x > maxSize.x) {
				maxSize.x = temp.x;
			}
			else if (temp.x < minSize.x) {
				minSize.x = temp.x;
			}
			if (temp.y > maxSize.y) {
				maxSize.y = temp.x;
			}
			else if (temp.y < minSize.y) {
				minSize.y = temp.y;
			}

			if (temp.z > maxSize.z) {
				maxSize.z = temp.z;
			}
			else if (temp.z < minSize.z) {
				minSize.z = temp.z;
			}
			vertices.push_back(temp);
			vertexData.push_back(temp);
		}
		else if (strstr(inputString, "f") != nullptr) {
			//face data

			MeshFace temp;
				sscanf_s(inputString, "f %u/%u/%u %u/%u/%u %u/%u/%u",
					&temp.vertices[0], &temp.textureUVs[0], &temp.normals[0],
					&temp.vertices[1], &temp.textureUVs[1], &temp.normals[1],
					&temp.vertices[2], &temp.textureUVs[2], &temp.normals[2]);
			faceData.push_back(temp);
		}

	}
	input.close();

	_NumFaces = faceData.size();
	_NumVertices = _NumFaces * 3;

	BoundingBox.m_size = glm::vec3(maxSize.x - minSize.x, maxSize.y - minSize.y, maxSize.z - minSize.z);
	BoundingBox.m_center = glm::vec3((minSize.x + maxSize.x) / 2, (minSize.y + maxSize.y) / 2, (minSize.z + maxSize.z) / 2);

	return true;
}

