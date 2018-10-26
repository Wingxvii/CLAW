#include "Transform.h"

Transform::Transform()
{
	position = glm::vec3(0, 0, 0);
	rotation = glm::vec3(0, 0, 0);
	scale = glm::vec3(1, 1, 1);

}

Transform::~Transform()
{
}

glm::vec3 Transform::getBackward()
{ 
	glm::vec3 back = glm::vec3(0, 0, 0);
	//find back direction
	back.x = sin(glm::radians(rotation.y));
	back.y = cos(glm::radians(rotation.y));
	back = glm::normalize(back);

	return back;
}

glm::vec3 Transform::getLeft()
{
	glm::vec3 left = glm::normalize(glm::cross(getBackward(), glm::vec3(0.0f, 1.0f, 0.0f)));

	return left;
}
