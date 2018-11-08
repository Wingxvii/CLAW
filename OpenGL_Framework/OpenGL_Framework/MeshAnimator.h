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

	std::vector<std::vector<Mesh>> animations;
	std::vector<Mesh> frames;
	Mesh interpolatedMesh;
	int frameIndex = 0;
	int targetframe = 0;
	float frameDuration = 0.1f;
	float timer = 0;
	bool loops = true;
	bool bound = false;

	//The animations are held in 2d vector this index is for which animation you want to play 
	//this is based on the order they are loades in. i.e. idle animations would be index 0 if they are loaded first 
	void playAnimations(float dt, int index);
	bool loadMeshes(std::string meshPrefix, int numOfFrames);


private:

	void bindCurrentMesh();
	void releaseCurrentMesh();

	
	
};

