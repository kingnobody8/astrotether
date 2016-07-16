#pragma once
#include "actor.h"

class EntityActor : public Actor
{
public:
	EntityActor() : m_pVActors(nullptr) {}
	virtual ~EntityActor() {}

	virtual void OnCollision(EntityActor* pOther) {}

	void SetActorList(std::vector<Actor*>* pVActors) { m_pVActors = pVActors; }

protected:

	std::vector<Actor*>* m_pVActors;
};