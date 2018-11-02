#pragma once
#include "include/glm/glm.hpp"
#include "Transform.h"

enum ColliderTag {

	PLAYER = 0,
	BREAKABLE = 1,
	NON_BREAKABLE = 2,

};

class BoxCollider
{
public:
	BoxCollider();
	BoxCollider(ColliderTag tag, glm::vec3 size, glm::vec3 center, int index);
	~BoxCollider();

	bool checkCollision(BoxCollider otherObj);

	ColliderTag tag;
	glm::vec3 size;
	glm::vec3 center;
	int index;

	glm::vec3 offset;

};
