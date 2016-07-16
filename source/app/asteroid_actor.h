#pragma once
#include "entity_actor.h"
#include "box2d.h"
#include "engine/physics/debug_draw.h"

class AsteroidActor : public EntityActor
{
public:
	enum EAstroSize { EAS_INVALID = -1, EAS_SMALL, EAS_MED, EAS_BIG, EAS_COUNT };

	AsteroidActor();
	virtual ~AsteroidActor();

	void Create(b2World* pWorld, b2Vec2 pos, b2Vec2 dir, EAstroSize size, std::vector<Actor*>* pActors);

	virtual void Update(float dt);
	virtual void Draw(sf::RenderWindow& window);

	virtual void OnCollision(EntityActor* pOther);
	void Split();
	void Destroy();

	void SetB2Body(b2Body* pBody) { m_pBody = pBody; m_pBody->SetUserData(this); }
	b2Body* GetB2Body() const { return m_pBody; }

	void SetSize(const EAstroSize& eSize) { m_nSize = eSize; }
	const EAstroSize& GetSize() const { return (EAstroSize)m_nSize; }

protected:
	b2Body* m_pBody;
	engine::RenderVerts verts;
	int m_nSize;
	bool m_bDestroy;
	bool m_bSplit;
	bool m_bShipHitThisFrame;
};