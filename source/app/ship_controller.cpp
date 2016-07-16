#include "ship_controller.h"
#include "ship_actor.h"
#include <SFML/Window.hpp>

const float ROTATE_VEL = 10.0f;
const float ACC_VEL = 1000000.0f;

ShipController::ShipController(ShipActor* pShipActor)
	: m_pShipActor(pShipActor)
{
}

ShipController::~ShipController()
{}

void ShipController::Update(float dt)
{
	b2Body* pBody = m_pShipActor->GetB2Body();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
	{
		pBody->SetTransform(pBody->GetPosition(), pBody->GetAngle() + ROTATE_VEL * dt);
		pBody->SetAngularVelocity(0);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
	{
		pBody->SetTransform(pBody->GetPosition(), pBody->GetAngle() - ROTATE_VEL * dt);
		pBody->SetAngularVelocity(0);
	}

	//Get ship's direction
	float angle = pBody->GetAngle();
	angle += 3.14159f / 2.0f;
	b2Vec2 dir(cos(angle), sin(angle));
	dir.Normalize();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
	{
		pBody->ApplyForceToCenter(dir * ACC_VEL * dt, true);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
	{
		pBody->ApplyForceToCenter(-dir * ACC_VEL * dt, true);
	}

}

