#include "MeshAnimator.h"


#define BUFFER_OFFSET(i) ((char *)0 + (i))

MeshAnimator::MeshAnimator()
{
}


MeshAnimator::~MeshAnimator()
{
}

void MeshAnimator::update(float dt)
{
	if (bound && frames.size() != 1) {
		releaseCurrentMesh();
		bound = false;
	}

	timer += dt;
	if (frames.size() == 1) {
		interpolatedMesh = frames[0];
	}
	else {
	

		for (int i = 0; i < interpolatedMesh._NumVertices; i++) {
			interpolatedMesh.vertices[i] = frames[frameIndex].vertices[i] * (1.0f - g_mInterp) + frames[frameIndex+1].vertices[i] * g_mInterp;
		}


		

	}

	
}

bool MeshAnimator::loadMeshes(std::string meshPrefix, int numOfFrames)
{
	bool worked;
	for (int i = 0; i < numOfFrames; i++) {
		Mesh temp;
		worked = temp.LoadfromFile(meshPrefix + std::to_string(i));

		frames.push_back(temp);
	}

	return worked;
}


void MeshAnimator::bindCurrentMesh()
{
	std::vector<float> unPackedVertexData;
	std::vector<float> unPackedTextureData;
	std::vector<float> unPackedNormalData;

	for (unsigned i = 0; i < interpolatedMesh.faceData.size(); i++) {
		for (unsigned j = 0; j < 3; j++) {
			unPackedVertexData.push_back(interpolatedMesh.vertexData[interpolatedMesh.faceData[i].vertices[j] - 1].x);
			unPackedVertexData.push_back(interpolatedMesh.vertexData[interpolatedMesh.faceData[i].vertices[j] - 1].y);
			unPackedVertexData.push_back(interpolatedMesh.vertexData[interpolatedMesh.faceData[i].vertices[j] - 1].z);

			unPackedTextureData.push_back(interpolatedMesh.textureData[interpolatedMesh.faceData[i].textureUVs[j] - 1].x);
			unPackedTextureData.push_back(interpolatedMesh.textureData[interpolatedMesh.faceData[i].textureUVs[j] - 1].y);


			unPackedNormalData.push_back(interpolatedMesh.normalData[interpolatedMesh.faceData[i].normals[j] - 1].x);
			unPackedNormalData.push_back(interpolatedMesh.normalData[interpolatedMesh.faceData[i].normals[j] - 1].y);
			unPackedNormalData.push_back(interpolatedMesh.normalData[interpolatedMesh.faceData[i].normals[j] - 1].z);

		}
	}

	interpolatedMesh._NumFaces = interpolatedMesh.faceData.size();
	interpolatedMesh._NumVertices = interpolatedMesh._NumFaces * 3;

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
	unPackedNormalData.clear();
	unPackedTextureData.clear();
	unPackedVertexData.clear();

	bound = true;
}

void MeshAnimator::releaseCurrentMesh()
{
	glDeleteBuffers(1, &VBO_Normals);
	glDeleteBuffers(1, &VBO_UVs);
	glDeleteBuffers(1, &VBO_Verticies);

	VBO_Normals = 0;
	VBO_UVs = 0;
	VBO_Verticies = 0;
	VAO = 0;
	
}
