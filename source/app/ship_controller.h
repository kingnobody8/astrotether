#pragma once

#include "actor_controller.h"

//forward declare
class ShipActor;

class ShipController : public ActorController
{
public:
	ShipController(ShipActor* player);
	virtual ~ShipController();

	virtual void Update(float dt);

private:
	ShipActor* m_pShipActor;
};