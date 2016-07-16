#include "asteroid_actor.h"

const int BIG_RADIUS = 7.0f;
const int MED_RADIUS = 5.0f;
const int SMALL_RADIUS = 1.0f;

const float SPLIT_SPEED_MIN = 10.0f;
const float SPLIT_SPEED_MAX = 25.0f;

AsteroidActor::AsteroidActor()
	: m_pBody(nullptr)
	, m_nSize(EAS_INVALID)
	, m_bDestroy(false)
	, m_bSplit(false)
	, m_bShipHitThisFrame(false)
{
	m_szType = "AsteroidActor";
}

AsteroidActor::~AsteroidActor()
{}

void AsteroidActor::Create(b2World* pWorld, b2Vec2 pos, b2Vec2 dir, EAstroSize size, std::vector<Actor*>* pActors)
{
	m_pVActors = pActors;

	dir.Normalize();
	float speed = rand() % (int)(SPLIT_SPEED_MAX - SPLIT_SPEED_MIN) + SPLIT_SPEED_MIN;
	dir *= speed;

	b2BodyDef body_def;
	body_def.active = true;
	body_def.allowSleep = true;
	body_def.angle = 0;
	body_def.angularDamping = 0;
	body_def.angularVelocity = rand() % 100 / 100.0f - 0.5f;
	body_def.awake = true;
	body_def.bullet = true;
	body_def.fixedRotation = false;
	body_def.gravityScale = 1.0f;
	body_def.linearDamping = 0.0f;
	body_def.linearVelocity = dir;
	body_def.position = pos;
	body_def.type = b2BodyType::b2_dynamicBody;
	body_def.userData = this;

	m_pBody = pWorld->CreateBody(&body_def);

	b2CircleShape shapec;
	switch (size)
	{
	case EAS_BIG: shapec.m_radius = BIG_RADIUS; break;
	case EAS_MED: shapec.m_radius = MED_RADIUS; break;
	case EAS_SMALL: shapec.m_radius = SMALL_RADIUS; break;
	}
	b2FixtureDef fdef;
	fdef.density = 10.0f;
	fdef.shape = &shapec;
	fdef.restitution = 0.5f;
	m_pBody->CreateFixture(&fdef);

	m_nSize = size;
}

void AsteroidActor::Update(float dt)
{
	m_bShipHitThisFrame = false;

	if (m_bDestroy)
	{
		Destroy();
		return;
	}
	else if (m_bSplit)
	{
		Split();
	}
}

void AsteroidActor::Draw(sf::RenderWindow& window)
{
	if (m_pBody == nullptr)
		return;

	//Get ship's direction
	float angle = m_pBody->GetAngle();
	angle += 3.14159f / 2.0f;
	b2Vec2 dir(cos(angle), sin(angle));
	dir.Normalize();

	verts.m_pRenWin = &window;
	verts.Vertex(sf::Vector2f(m_pBody->GetPosition().x,					m_pBody->GetPosition().y),				sf::Color::Green);
	verts.Vertex(sf::Vector2f(m_pBody->GetPosition().x + dir.x * 3,	m_pBody->GetPosition().y + dir.y * 3), sf::Color::Green);
	verts.Flush();
}

void AsteroidActor::OnCollision(EntityActor* pOther)
{
	if (m_bDestroy)
		return;

	if (pOther->GetType() == "AsteroidActor" && !m_bShipHitThisFrame)
	{
		m_bShipHitThisFrame = true;
		m_nSize--;
		if (m_nSize == EAS_INVALID)
		{
			m_bDestroy = true;
		}
		else
		{
			m_bSplit = true;
		}
	}
}

void AsteroidActor::Split()
{
	m_bSplit = false;

	b2World* pWorld = m_pBody->GetWorld();

	/*b2BodyDef def;
	def.active = m_pBody->IsActive();
	def.allowSleep = m_pBody->IsSleepingAllowed();
	def.angle = m_pBody->GetAngle();
	def.angularDamping = m_pBody->GetAngularDamping();
	def.angularVelocity = m_pBody->GetAngularVelocity();
	def.awake = m_pBody->IsAwake();
	def.bullet = m_pBody->IsBullet();
	def.fixedRotation = m_pBody->IsFixedRotation();
	def.gravityScale = m_pBody->GetGravityScale();
	def.linearDamping = m_pBody->GetLinearDamping();
	def.linearVelocity = m_pBody->GetLinearVelocity();
	def.position = m_pBody->GetPosition();
	def.position.x += rand() % 100 - 50;
	def.position.y += rand() % 100 - 50;
	def.type = m_pBody->GetType();
	def.userData = nullptr;

	b2Body* pTemp = pWorld->CreateBody(&def);

	b2Fixture* pFixture = m_pBody->GetFixtureList();
	while (pFixture != nullptr)
	{
	b2FixtureDef fdef;
	fdef.density = pFixture->GetDensity();
	fdef.filter = pFixture->GetFilterData();
	fdef.friction = pFixture->GetFriction();
	fdef.isSensor = pFixture->IsSensor();
	fdef.restitution = pFixture->GetRestitution();
	fdef.shape = pFixture->GetShape();
	fdef.userData = nullptr;

	pTemp->CreateFixture(&fdef);

	pFixture = pFixture->GetNext();
	}*/
	b2Vec2 pos = m_pBody->GetPosition();
	int radius = 0;
	switch (m_nSize)
	{
	case EAS_BIG: radius = BIG_RADIUS; break;
	case EAS_MED: radius = MED_RADIUS; break;
	case EAS_SMALL: radius = SMALL_RADIUS; break;
	}
	b2Vec2 dir(rand() % 100 - 50, rand() % 100 - 50);
	dir.Normalize();
	dir *= radius * 1.5;

	for (int i = 0; i < 2; ++i)
	{
		AsteroidActor* pAstroActor = new AsteroidActor();
		pAstroActor->Create(m_pBody->GetWorld(), pos + dir, dir, (EAstroSize)m_nSize, m_pVActors);
		m_pVActors->push_back(pAstroActor);
		dir *= -1;
	}
	Destroy();
}

void AsteroidActor::Destroy()
{
	m_bDestroy = false;
	m_pBody->GetWorld()->DestroyBody(m_pBody);
	m_pBody = nullptr;

	for (int i = 0; i < m_pVActors->size(); ++i)
	{
		if ((*m_pVActors)[i] == this)
		{
			(*m_pVActors).erase((*m_pVActors).begin() + i);
			break;
		}
	}

	delete this;
}