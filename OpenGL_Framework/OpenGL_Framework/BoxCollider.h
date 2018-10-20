#pragma once
#include <glm/ext.hpp>

class BoxCollider
{
public:
	BoxCollider();
	BoxCollider(glm::vec3 p_maxBound, glm::vec3 p_minBound);
	~BoxCollider();

	void update(float deltaTime);

private:
	bool drawBoundingBox = false;
	
	void draw();

	glm::vec3 m_maxBound;
	glm::vec3 m_minBound;

	float width;
	float height;
	float depth;

	glm::vec3 center;

};

