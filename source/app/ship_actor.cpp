#include "ship_actor.h"

ShipActor::ShipActor()
	: m_pBody(nullptr)
{
	m_szType = "SpriteActor";
	m_pController = new ShipController(this);
}

ShipActor::~ShipActor()
{}

void ShipActor::Update(float dt)
{
	m_pController->Update(dt);
}

void ShipActor::Draw(sf::RenderWindow& window)
{
	//Get ship's direction
	float angle = m_pBody->GetAngle();
	angle += 3.14159f / 2.0f;
	b2Vec2 dir(cos(angle), sin(angle));
	dir.Normalize();

	verts.m_pRenWin = &window;
	verts.Vertex(sf::Vector2f(m_pBody->GetPosition().x,					m_pBody->GetPosition().y),				sf::Color::Red);
	verts.Vertex(sf::Vector2f(m_pBody->GetPosition().x + dir.x * 10,	m_pBody->GetPosition().y + dir.y * 10), sf::Color::Red);
	verts.Flush();
}

