#pragma once
#include "Transform.h"

enum class EntityTypes {
	BASE_ENTITY = 0,
	CAMERA = 1,
	PLAYER = 2,
};

class Entity
{
public:
	Entity();
	~Entity();

	Transform* getTransform() const;
	int m_entityType = (int)EntityTypes::BASE_ENTITY;
	
private:
	
	Transform* m_trans = new Transform();
};

