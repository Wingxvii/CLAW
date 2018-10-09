#pragma once
#include "GL\glew.h"

#include <string>
#include <vector>
#include "glm/glm.hpp"

class Mesh {
public:
	Mesh();
	~Mesh();

	Mesh(const std::string &file);

	//load a mesh and send to opengl
	bool LoadfromFile(const std::string &file);

	//release data from opengl
	void Unload();

	unsigned int GetNumFaces() const;
	unsigned int GetNumVertices() const;

	//buffers and objects
	GLuint VBO_Verticies = GL_NONE;
	GLuint VBO_UVs = GL_NONE;
	GLuint VBO_Normals = GL_NONE;
	GLuint VAO = GL_NONE;

private:
	unsigned int _NumFaces = 0;
	unsigned int _NumVertices = 0;
};