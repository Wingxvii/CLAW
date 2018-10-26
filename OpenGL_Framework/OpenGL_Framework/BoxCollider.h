#pragma once
#include <glm/ext.hpp>
#include "Transform.h"

#include "GL\glew.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

class BoxCollider 
{
public:
	BoxCollider();
	BoxCollider(glm::vec3 p_maxBound, glm::vec3 p_minBound);
	~BoxCollider();

	void update(float deltaTime);

	glm::vec3 m_maxBound;
	glm::vec3 m_minBound;

	Transform* boxTransform = new Transform();
	void draw(Transform* Meshtransform, float NumVertices);

private:
	bool drawBoundingBox = false;
	
	void drawLine(glm::vec3 p0, glm::vec3 p1, float lineWidth, glm::vec4 color);
	void line(glm::vec3 p0, glm::vec3 p1, glm::vec4 color);//use alpha blend or not
	

	glm::vec3 size();
	glm::vec3 centre;
	
	glm::vec3 center;
};

