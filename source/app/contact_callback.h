#pragma once
#include "box2d/box2d.h"
#include "entity_actor.h"

class ContactCallback : public b2ContactListener
{
public:
	ContactCallback()
	{
	}

	virtual void BeginContact(b2Contact* contact)
	{
		void* userDataA = contact->GetFixtureA()->GetBody()->GetUserData();
		if (userDataA == nullptr)
			return;
		void* userDataB = contact->GetFixtureB()->GetBody()->GetUserData();
		if (userDataB == nullptr)
			return;

		EntityActor* entA = static_cast<EntityActor*>(userDataA);
		EntityActor* entB = static_cast<EntityActor*>(userDataB);

		entA->OnCollision(entB);
		entB->OnCollision(entA);
	}

	virtual void EndContact(b2Contact* contact)
	{

	}

};
