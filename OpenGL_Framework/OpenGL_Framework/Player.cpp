#include "Player.h"



Player::Player()
{
	m_entityType = (int)EntityTypes::PLAYER;
}

Player::Player(Mesh* p_mesh) : PhysicalEntity(p_mesh)
{
	m_entityType = (int)EntityTypes::PLAYER;
}


Player::~Player()
{
}
