#pragma once
#include "entity_actor.h"

class GhostActor : public EntityActor //make this a template
{
public:
	GhostActor(EntityActor* pEnt);
	virtual ~GhostActor() {}

	virtual void Update(float dt);

protected:
	EntityActor* m_pHaunted;
	

};