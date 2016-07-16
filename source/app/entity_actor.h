#pragma once
#include "actor.h"
#include "box2d.h"

class EntityActor : public Actor
{
public:
	EntityActor() : m_pVActors(nullptr), m_pBody(nullptr) {}
	virtual ~EntityActor() {}

	virtual void Update(float dt)
	{
		b2Vec2 pos = m_pBody->GetPosition();

		if (pos.x > 64)
		{
			pos.x = -64 + (pos.x - 64);
		}
		if (pos.x < -64)
		{
			pos.x = 64 - (pos.x + 64);
		}

		if (pos.y > 64)
		{
			pos.y = -64 + (pos.y - 64);
		}
		if (pos.y < -64)
		{
			pos.y = 64 - (pos.y + 64);
		}

		m_pBody->SetTransform(pos, m_pBody->GetAngle());
	}

	virtual void OnCollision(EntityActor* pOther) {}

	void SetActorList(std::vector<Actor*>* pVActors) { m_pVActors = pVActors; }

	void SetB2Body(b2Body* pBody) { m_pBody = pBody; m_pBody->SetUserData(this); }
	b2Body* GetB2Body() const { return m_pBody; }

protected:
	b2Body* m_pBody;
	std::vector<Actor*>* m_pVActors;
};