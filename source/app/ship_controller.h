#pragma once
#include "actor_controller.h"
#include "engine/input/input_event.h"

//forward declare
class ShipActor;

class ShipController : public ActorController
{
public:
	ShipController(ShipActor* player);
	virtual ~ShipController();

	virtual void Update(float dt);

	void OnMouseButtonUp(engine::mouse_events::ButtonAction action);
	void KillTheRope();

private:
	ShipActor* m_pShipActor;
	util::Subscriber m_sub;
	b2RopeJoint* m_pRopeJoint;
};