#pragma once
#include "Transform.h"

enum class EntityTypes {
	PLAYER = 0,
	BREAKABLE = 1,
	NONBREAKABLE = 2,
	DEFAULT
};

class Entity
{
public:
	Entity();
	~Entity();

	
	int m_entityType = (int)EntityTypes::DEFAULT;
	
	
private:
	
	
};

