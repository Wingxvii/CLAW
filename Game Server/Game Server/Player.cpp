#include "Player.h"

Player::Player()
{
	transform = Transform();
	rigidbody = Rigidbody(transform);
	playerNumber = 1;
	active = false;
	collider = nullptr;
}

Player::Player(int num)
{
	transform = Transform();
	rigidbody = Rigidbody(transform);
	playerNumber = num;
	active = false;
	collider = nullptr;
}

Player::~Player()
{
	//delete collider;
}
