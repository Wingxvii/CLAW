#pragma once

#include <string>

#include "glm/glm.hpp"
#include "glm/ext.hpp"

class Transform
{
public:
	float m_pScale;

	float m_pRotX = 0.0f, m_pRotY = 0.0f, m_pRotZ = 0.0f; // local rotation angles		 

	glm::vec3 m_pLocalPosition;
						 
	glm::mat4 m_pLocalRotation = glm::mat4(1.0);
	glm::mat4 m_pLocalToWorldMatrix = glm::mat4(1.0);


	Transform();
	~Transform();

	void setPosition(glm::vec3 newPosition);
	void setRotationAngleX(float newAngle);
	void setRotationAngleY(float newAngle);
	void setRotationAngleZ(float newAngle);
	void setScale(float newScale);

	glm::vec3 getPosition();
	float getRotationAngleY();

	glm::mat4 getLocalToWorldMatrix();

	// TODO: Implement this update function
	virtual void update(float dt);

	// Other Properties
	std::string name;
	glm::vec4 color;

	glm::vec3 forward;
};