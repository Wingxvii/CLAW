#pragma once
#include "PhysicalEntity.h"
class Player :
	public PhysicalEntity
{
public:
	Player();
	Player(Mesh* p_mesh);
	~Player();
};

