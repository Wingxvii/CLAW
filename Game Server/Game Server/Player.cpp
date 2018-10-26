#include "Player.h"

Player::Player()
{
	rigidbody = Rigidbody();
	transform = Transform();
	playerNumber = 1;
}

Player::Player(int num)
{
	rigidbody = Rigidbody();
	transform = Transform();
	playerNumber = num;
}

Player::~Player()
{
}
