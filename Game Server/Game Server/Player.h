#pragma once
#include "include/glm/glm.hpp"
#include "Rigidbody.h"
#include "Transform.h"
#include "BoxCollider.h"
#include <string>
#include <vector>

enum class PlayerState {

	IDLE = 0,
	JUMP = 1

};

class Player {
public:
	Player();
	Player(int num);
	~Player();

	Rigidbody rigidbody;
	Transform transform;
	BoxCollider* collider;

	BoxCollider hitBox;

	int playerNumber;
	bool active;

	//jump
	int jumpPower;
	PlayerState state;

	std::vector<std::string> startData;
};