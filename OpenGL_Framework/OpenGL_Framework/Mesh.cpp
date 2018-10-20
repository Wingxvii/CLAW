#include "Mesh.h"
#include <fstream>
#include <iostream>


using namespace std;

#define CHAR_BUFFER_SIZE 128
#define BUFFER_OFFSET(i) ((char *)0 + (i))


struct MeshFace {
	MeshFace() {
		vertices[0] = 0;
		vertices[1] = 0;
		vertices[2] = 0;

		textureUVs[0] = 0;
		textureUVs[1] = 0;
		textureUVs[2] = 0;

		normals[0] = 0;
		normals[1] = 0;
		normals[2] = 0;
	}

	MeshFace(unsigned v1, unsigned v2, unsigned v3,
		unsigned t1, unsigned t2, unsigned t3,
		unsigned n1, unsigned n2, unsigned n3)
	{
		vertices[0] = v1;
		vertices[1] = v2;
		vertices[2] = v3;

		textureUVs[0] = t1;
		textureUVs[1] = t2;
		textureUVs[2] = t3;

		normals[0] = n1;
		normals[1] = n2;
		normals[2] = n3;

	}
	unsigned vertices[3];
	unsigned textureUVs[3];
	unsigned normals[3];
};

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
	vector<glm::vec3> vertexData;
	vector<glm::vec2> textureData;
	vector<glm::vec3> normalData;
	//face data
	vector<MeshFace> faceData;
	//unpacked data
	vector<float> unPackedVertexData;
	vector<float> unPackedTextureData;
	vector<float> unPackedNormalData;

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

	for (unsigned i = 0; i < faceData.size(); i++) {
		for (unsigned j = 0; j < 3; j++) {
			unPackedVertexData.push_back(vertexData[faceData[i].vertices[j] - 1].x);
			unPackedVertexData.push_back(vertexData[faceData[i].vertices[j] - 1].y);
			unPackedVertexData.push_back(vertexData[faceData[i].vertices[j] - 1].z);

			unPackedTextureData.push_back(textureData[faceData[i].textureUVs[j] - 1].x);
			unPackedTextureData.push_back(textureData[faceData[i].textureUVs[j] - 1].y);
			

			unPackedNormalData.push_back(normalData[faceData[i].normals[j] - 1].x);
			unPackedNormalData.push_back(normalData[faceData[i].normals[j] - 1].y);
			unPackedNormalData.push_back(normalData[faceData[i].normals[j] - 1].z);

		}
	}

	_NumFaces = faceData.size();
	_NumVertices = _NumFaces * 3;

	//Vertex array object
	glGenVertexArrays(1, &VAO);
	//bind to opengl
	glBindVertexArray(VAO);

	//Vertex buffer objects
	glGenBuffers(1, &VBO_Verticies);

	glGenBuffers(1, &VBO_UVs);

	glGenBuffers(1, &VBO_Normals);

	//stream 0 - verts
	//stream 1 - uvs
	//stream 2 - normals
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);


	glBindBuffer(GL_ARRAY_BUFFER, VBO_Verticies);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*unPackedVertexData.size(), &unPackedVertexData[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, BUFFER_OFFSET(0));

	glBindBuffer(GL_ARRAY_BUFFER, VBO_UVs);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*unPackedTextureData.size(), &unPackedTextureData[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, BUFFER_OFFSET(0));

	glBindBuffer(GL_ARRAY_BUFFER, VBO_Normals);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*unPackedNormalData.size(), &unPackedNormalData[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, BUFFER_OFFSET(0));

	//clean up
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	vertexData.clear();
	textureData.clear();
	normalData.clear();
	faceData.clear();
	unPackedNormalData.clear();
	unPackedTextureData.clear();
	unPackedVertexData.clear();

	return true;
}

void Mesh::Unload()
{
	glDeleteBuffers(1, &VBO_Normals);
	glDeleteBuffers(1, &VBO_UVs);
	glDeleteBuffers(1, &VBO_Verticies);

	VBO_Normals = 0;
	VBO_UVs = 0;
	VBO_Verticies = 0;
	VAO = 0;
	_NumFaces = 0;
	_NumVertices = 0;
}

unsigned int Mesh::GetNumFaces() const
{
	return _NumFaces;
}

unsigned int Mesh::GetNumVertices() const
{
	return _NumVertices;
}
