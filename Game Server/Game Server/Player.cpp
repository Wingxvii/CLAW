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
	health = 100;
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
	health = 100;
}

Player::~Player()
{
	//delete collider;
}

void Player::TickCoolDowns()
{
	if (CanLightAttack > 0) { CanLightAttack--; } // ticks light attack
	if (lightAttackFrames > 0) { lightAttackFrames--; } // ticks light attack

}

void Player::resetCooldowns()
{
	CanLightAttack = 0;
	lightAttackFrames = 0;
	jumpPower = 30;
}
