#include "MeshAnimator.h"


#define BUFFER_OFFSET(i) ((char *)0 + (i))

MeshAnimator::MeshAnimator()
{
}


MeshAnimator::~MeshAnimator()
{
}

void MeshAnimator::playAnimations(float dt, int animationIndex)
{
	if (animations[animationIndex].size() == 1) {
		drawMesh(frameIndex, frameIndex, animationIndex);
		return;
	}

	timer += dt;
	if (timer >= frameDuration) {
		timer = 0;
		frameIndex++;
	}

	if (frameIndex+1 >= animations[animationIndex].size()) {
		frameIndex = 0;
	}

	if (frameIndex == 5) {
		int here = 0;
	}

	else {

		drawMesh(frameIndex, frameIndex + 1, animationIndex);

		interpParam = timer / frameDuration;
	}
	
}

void MeshAnimator::runHealthAnimation()
{
	//health bar only has one animation with two frames 
	//that will be interpolated between
	drawMesh(frameIndex, frameIndex + 1, 0);
}

bool MeshAnimator::loadMeshes(std::string meshPrefix, int numOfFrames)
{
	bool worked;
	for (int i = 0; i < numOfFrames; i++) {
		Mesh temp;
		std::string base = "000000";
		std::string num = std::to_string(i);
		base.resize(base.size() - num.size());
		base.append(num);

		worked = temp.LoadfromFile(meshPrefix + base + ".obj");

		frames.push_back(temp);
	}
	if (worked) {
		animations.push_back(frames);
	}
	
	frames.clear();
	return worked;
}


void MeshAnimator::drawMesh(int frameIndex1, int frameIndex2, int animationIndex)
{
	//Vertex array object
	if (!isInit) {
		glGenVertexArrays(1, &VAO);	
	}
	//bind to opengl
	glBindVertexArray(VAO);

	if (!isInit) {
		//Vertex buffer objects
		glGenBuffers(1, &VBO_Verticies);
		glGenBuffers(1, &VBO_UVs);
		glGenBuffers(1, &VBO_Normals);
		//Second Frame
		glGenBuffers(1, &VBO_Verticies2);
		glGenBuffers(1, &VBO_UVs2);
		glGenBuffers(1, &VBO_Normals2);

		//stream 0 - verts
		//stream 1 - uvs
		//stream 2 - normals
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glEnableVertexAttribArray(3);
		glEnableVertexAttribArray(4);
		glEnableVertexAttribArray(5);
		isInit = true;
	}

	//Bind and Update Buffers 
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Verticies);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*animations[animationIndex][frameIndex].unPackedVertexData.size(), &animations[animationIndex][frameIndex].unPackedVertexData[0], GL_DYNAMIC_DRAW);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, BUFFER_OFFSET(0));

	glBindBuffer(GL_ARRAY_BUFFER, VBO_UVs);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*animations[animationIndex][frameIndex].unPackedTextureData.size(), &animations[animationIndex][frameIndex].unPackedTextureData[0], GL_DYNAMIC_DRAW);
	glVertexAttribPointer((GLuint)1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, BUFFER_OFFSET(0));

	glBindBuffer(GL_ARRAY_BUFFER, VBO_Normals);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*animations[animationIndex][frameIndex].unPackedNormalData.size(), &animations[animationIndex][frameIndex].unPackedNormalData[0], GL_DYNAMIC_DRAW);
	glVertexAttribPointer((GLuint)2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, BUFFER_OFFSET(0));

	glBindBuffer(GL_ARRAY_BUFFER, VBO_Verticies2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*animations[animationIndex][frameIndex2].unPackedVertexData.size(), &animations[animationIndex][frameIndex2].unPackedVertexData[0], GL_DYNAMIC_DRAW);
	glVertexAttribPointer((GLuint)3, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, BUFFER_OFFSET(0));

	glBindBuffer(GL_ARRAY_BUFFER, VBO_UVs2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*animations[animationIndex][frameIndex2].unPackedTextureData.size(), &animations[animationIndex][frameIndex2].unPackedTextureData[0], GL_DYNAMIC_DRAW);
	glVertexAttribPointer((GLuint)4, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, BUFFER_OFFSET(0));

	glBindBuffer(GL_ARRAY_BUFFER, VBO_Normals2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*animations[animationIndex][frameIndex2].unPackedNormalData.size(), &animations[animationIndex][frameIndex2].unPackedNormalData[0], GL_DYNAMIC_DRAW);
	glVertexAttribPointer((GLuint)5, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, BUFFER_OFFSET(0));

	//clean up
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	bound = true;
}

void MeshAnimator::releaseCurrentMesh()
{
	glDeleteBuffers(1, &VBO_Normals);
	glDeleteBuffers(1, &VBO_UVs);
	glDeleteBuffers(1, &VBO_Verticies);
	glDeleteVertexArrays(1, &VAO);

	VBO_Normals = 0;
	VBO_UVs = 0;
	VBO_Verticies = 0;
	VAO = 0;
	
	isInit = false;
	
}
