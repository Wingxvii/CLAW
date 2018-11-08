#pragma once
#include "GL\glew.h"

#include <string>
#include <vector>
#include "glm/glm.hpp"

#include "BoxCollider.h"
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

class Mesh {
public:
	Mesh();
	~Mesh();

	Mesh(const std::string &file);

	//load a mesh and send to opengl
	bool LoadfromFile(const std::string &file);

	//release data from opengl
	void Unload();

	

	//buffers and objects
	

	glm::vec3 maxSize{ 0.0f, 0.0f, 0.0f };
	glm::vec3 minSize{ 10000.0f, 10000.0f,10000.0f };

	BoxCollider BoundingBox;

	Transform* transform = new Transform();

	//std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> vertexData;
	std::vector<glm::vec2> textureData;
	std::vector<glm::vec3> normalData;

	//vector<MeshFace> faceData;
	std::vector<MeshFace> faceData;

	unsigned int _NumFaces = 0;
	unsigned int _NumVertices = 0;
};