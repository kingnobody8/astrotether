#pragma once
#include "entity_actor.h"
#include "engine/physics/debug_draw.h"
#include "ship_controller.h"

class ShipActor : public EntityActor
{
public:
	ShipActor();
	virtual ~ShipActor();

	virtual void Update(float dt);
	virtual void Draw(sf::RenderWindow& window);

	virtual void OnCollision(EntityActor* pOther);
	void KillTheRope();

protected:
	engine::RenderVerts verts;
	ShipController* m_pController;


	bool m_bDead;
};