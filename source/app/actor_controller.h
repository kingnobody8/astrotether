#pragma once

class ActorController
{
public:
	ActorController() {}
	virtual ~ActorController() {}

	virtual void Update(float dt) = 0;

};