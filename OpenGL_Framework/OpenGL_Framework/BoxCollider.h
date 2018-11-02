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
	~BoxCollider();

	glm::vec3 m_center;
	glm::vec3 m_size;

private:

};

