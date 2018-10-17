#pragma once
#include "Entity.h"
#include "Mesh.h"
class PhysicalEntity :
	public Entity
{
public:
	PhysicalEntity();
	PhysicalEntity(Mesh* p_mesh);
	~PhysicalEntity();

	Mesh* getMesh() const;
	void setMesh(Mesh* p_mesh);
private:
	Mesh* m_mesh = new Mesh();
};

