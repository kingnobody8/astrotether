#pragma once
#include "entity_actor.h"
#include "box2d.h"
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

	void SetB2Body(b2Body* pBody) { m_pBody = pBody; m_pBody->SetUserData(this); }
	b2Body* GetB2Body() const { return m_pBody; }

protected:
	b2Body* m_pBody;
	engine::RenderVerts verts;
	ShipController* m_pController;


	bool m_bDead;
};