#pragma once
#include "include/glm/glm.hpp"
#include "Rigidbody.h"
#include "Transform.h"
#include "BoxCollider.h"
#include <string>
#include <vector>

enum class PlayerState {

	IDLE = 0,
	JUMP = 1,
	PARRY = 2,

};

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

	int health;

	//jump
	int jumpPower;
	PlayerState state;

	//attack cooldowns
	int CanLightAttack = 0;
	int lightAttackFrames = 0;
	void TickCoolDowns();
	void resetCooldowns();

	glm::vec3 goingDirection{0.0f, 0.0f, 0.0f};

	std::vector<std::string> startData;
};