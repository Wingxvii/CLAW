#include "PhysicalEntity.h"


PhysicalEntity::PhysicalEntity()
{
}

PhysicalEntity::PhysicalEntity(Mesh* p_mesh)
{
	m_mesh = p_mesh;
}

PhysicalEntity::~PhysicalEntity()
{
}

Mesh * PhysicalEntity::getMesh() const
{
	return m_mesh;
}

void PhysicalEntity::setMesh(Mesh* p_mesh)
{
	m_mesh = p_mesh;
}
