#include "Player.h"

Player::Player()
{
	rigidbody = Rigidbody();
	transform = Transform();
	playerNumber = 1;
	active = false;
}

Player::Player(int num)
{
	rigidbody = Rigidbody();
	transform = Transform();
	playerNumber = num;
	active = false;
}

Player::~Player()
{
}
