#include "Player.h"

Player::Player()
{
	transform = Transform();
	rigidbody = Rigidbody(transform);
	playerNumber = 1;
	active = false;
	collider = nullptr;
	state = PlayerState::IDLE;
	jumpPower = 30;
	hitBox = BoxCollider(ColliderTag::TRIGGER, glm::vec3(8.0f, 4.0f, 8.0f), glm::vec3(0.0f, 0.0f, 0.0f),-1);
}

Player::Player(int num)
{
	transform = Transform();
	rigidbody = Rigidbody(transform);
	playerNumber = num;
	active = false;
	collider = nullptr;
	state = PlayerState::IDLE;
	jumpPower = 30;
	hitBox = BoxCollider(ColliderTag::TRIGGER, glm::vec3(8.0f, 4.0f, 8.0f), glm::vec3(0.0f, 0.0f, 0.0f), -1);
}

Player::~Player()
{
	//delete collider;
}
