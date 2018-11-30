#pragma once
#include "Mesh.h"
#include <vector>

class MeshAnimator
{
public:
	MeshAnimator();
	~MeshAnimator();

	GLuint VAO = GL_NONE;
	GLuint VBO_Verticies = GL_NONE;
	GLuint VBO_UVs = GL_NONE;
	GLuint VBO_Normals = GL_NONE;
	GLuint VBO_Verticies2 = GL_NONE;
	GLuint VBO_UVs2 = GL_NONE;
	GLuint VBO_Normals2 = GL_NONE;

	std::vector<std::vector<Mesh>> animations;
	std::vector<Mesh> frames;
	int frameIndex = 0;
	int targetframe = 0;
	float frameDuration = 0.15f;
	float timer = 0;
	float interpParam = 0.0f;
	bool loops = false;
	bool bound = false;
	bool isInit = false;
	bool finished = false;

	//The animations are held in 2d vector this index is for which animation you want to play 
	//this is based on the order they are loades in. i.e. idle animations would be index 0 if they are loaded first 
	void playAnimations(float dt, int index);
	//only use this for the health bar anything else will break
	void playAnimationOnce(float dt, int index);
	void runHealthAnimation();
	bool loadMeshes(std::string meshPrefix, int numOfFrames);


private:

	void drawMesh(int frameIndex1, int frameIndex2, int animationIndex);
	void releaseCurrentMesh();

	
	
};

