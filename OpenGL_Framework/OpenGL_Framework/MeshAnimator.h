#pragma once
#include "Mesh.h"
#include <vector>

class MeshAnimator
{
public:
	MeshAnimator();
	~MeshAnimator();

	GLuint VBO_Verticies = GL_NONE;
	GLuint VBO_UVs = GL_NONE;
	GLuint VBO_Normals = GL_NONE;
	GLuint VAO = GL_NONE;

	std::vector<Mesh> frames;
	Mesh interpolatedMesh;
	int frameIndex = 0;
	float frameDuration = 1;
	float timer = 0;
	bool loops = true;
	bool bound = false;

	void update(float dt);
	bool loadMeshes(std::string meshPrefix, int numOfFrames);


private:

	void bindCurrentMesh();
	void releaseCurrentMesh();

	
	
};

