#include "BoxCollider.h"

BoxCollider::BoxCollider()
{
	tag = BREAKABLE;
	size = glm::vec3(0.0f,0.0f,0.0f);
	center = glm::vec3(0.0f, 0.0f, 0.0f);
	index = -1;
}

BoxCollider::BoxCollider(ColliderTag tag, glm::vec3 size, glm::vec3 center, int index)
{
	this->tag = tag;
	this->size = size;
	this->center = center;
	this->index = index;
}

BoxCollider::~BoxCollider()
{
}

//Checks Collision for Axis Alligned Bounding boxes
bool BoxCollider::checkCollision(BoxCollider otherObj)
{
	//check X axis
	if ( glm::abs(center.x - otherObj.center.x) < (size.x/2) + (otherObj.size.x/2))
	{
		//check Y axis
		if (glm::abs(center.y - otherObj.center.y) < (size.y / 2) + (otherObj.size.y / 2))
		{
			//check Z axis
			if (glm::abs(center.z - otherObj.center.z) < (size.z / 2) + (otherObj.size.z / 2))
			{
				return true;
			}
		}
	}

	return false;
}
