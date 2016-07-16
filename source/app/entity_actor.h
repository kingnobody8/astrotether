#pragma once
#include "actor.h"

class EntityActor : public Actor
{
public:
	EntityActor() {}
	virtual ~EntityActor() {}

	virtual void OnCollision(EntityActor* pOther) {}
protected:
};