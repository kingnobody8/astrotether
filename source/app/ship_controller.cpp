#include "ship_controller.h"
#include "ship_actor.h"
#include <SFML/Window.hpp>

#include "engine/physics/callbacks/raycast_callback.h"

const float ROTATE_VEL = 10.0f;
const float ACC_VEL = 1000000.0f;

ShipController::ShipController(ShipActor* pShipActor)
	: m_pShipActor(pShipActor)
	, m_pRopeJoint(nullptr)
{
	engine::mouse_events::s_InputMouseButtonUp.Subscribe(&m_sub, BIND1(this, &ShipController::OnMouseButtonUp));
}

ShipController::~ShipController()
{
	m_sub.UnsubscribeAll();
}

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

void ShipController::OnMouseButtonUp(engine::mouse_events::ButtonAction action)
{
	if (action.m_event.mouseButton.button == sf::Mouse::Button::Right)
	{
		if (m_pRopeJoint != nullptr)
		{
			m_pShipActor->GetB2Body()->GetWorld()->DestroyJoint(m_pRopeJoint);
			m_pRopeJoint = NULL;
		}
		else
		{
			b2Vec2 pw;
			pw.x = action.m_pixel.x;
			pw.y = action.m_pixel.y;

			//do a raycast
			engine::physics::callbacks::RayCastClosestCallback callback;
			b2Vec2 direction = pw - m_pShipActor->GetB2Body()->GetPosition();
			direction.Normalize();
			direction *= 100;
			b2Vec2 point = m_pShipActor->GetB2Body()->GetPosition() + direction;
			m_pShipActor->GetB2Body()->GetWorld()->RayCast(&callback, m_pShipActor->GetB2Body()->GetPosition(), point);

			if (callback.m_bHit)
			{
				b2Body* body = callback.m_pFixture->GetBody();
				b2RopeJointDef md;
				md.bodyA = m_pShipActor->GetB2Body();
				md.bodyB = body;
				md.localAnchorA.SetZero();
				md.localAnchorB = body->GetLocalPoint(callback.m_point);
				md.maxLength = (callback.m_point - m_pShipActor->GetB2Body()->GetPosition()).Length() * 1.1f;
				md.collideConnected = true;
				m_pRopeJoint = (b2RopeJoint*)m_pShipActor->GetB2Body()->GetWorld()->CreateJoint(&md);
				body->SetAwake(true);
			}
		}
	}
}

void ShipController::KillTheRope()
{
	m_pRopeJoint = nullptr;
}