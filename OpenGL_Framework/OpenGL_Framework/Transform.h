#pragma once

#include <string>

#include "glm/glm.hpp"
#include "glm/ext.hpp"

class Transform
{
public:
	glm::vec3 m_pScale;

	glm::vec3 m_pRotation;

	glm::vec3 m_pLocalPosition;
						 
	glm::mat4 m_pLocalRotation = glm::mat4(1.0);
	glm::mat4 m_pLocalToWorldMatrix = glm::mat4(1.0);


	Transform();
	~Transform();

	void setPosition(glm::vec3 newPosition);
	void setRotation(glm::vec3 newAngle);
	void setScale(float newScale);
	void setScale(glm::vec3 newScale);

	glm::vec3 getPosition();
	float getRotationAngleY();
	glm::mat4 getLocalToWorldMatrix();

	// TODO: Implement this update function
	virtual void update(float dt);

	// Other Properties
	std::string name;
	glm::vec4 color;

	glm::vec3 forward;
	glm::vec3 right;

};