#include "Entity.h"



Entity::Entity()
{
}


Entity::~Entity()
{
}

Transform* Entity::getTransform() const
{
	return m_trans;
}
