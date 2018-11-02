#pragma once
#include "include/glm/glm.hpp"
#include "Rigidbody.h"
#include "Transform.h"
#include "BoxCollider.h"

class Player {
public:
	Player();
	Player(int num);
	~Player();

	Rigidbody rigidbody;
	Transform transform;
	BoxCollider* collider;
	int playerNumber;
	bool active;


};