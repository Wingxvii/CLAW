#pragma once
#include "include/glm/glm.hpp"
#include "Rigidbody.h"
#include "Transform.h"

class Player {
public:
	Player();
	Player(int num);
	~Player();

	Rigidbody rigidbody;
	Transform transform;
	int playerNumber;
	bool active;


};