#pragma once
#include "player_ent.h"
#include "render/render_plugin.h"
#include "utility/helper/func.h"
#include <string>
#include <regex>
#include "state/state_plugin.h"
#include "states/testbed_state.h"
#include "physics/physics_plugin.h"
#include "physics/callbacks/raycast_callback.h"
#include "utility/resource/json.h"
#include "rapidjson/FileReadStream.h"

const std::string path = "../spine_examples/REP/export/REP";
//const std::string path = "assets/animations/REP/export/REP";
const std::string file = "spineboy";
spine::AnimationStateData* pData = null;

namespace app
{
	namespace entity
	{
		void PlayerValue::LoadValues(const std::string& file_path)
		{
			FILE* fp = fopen(file_path.c_str(), "rb"); // non-Windows use "r"
			char readBuffer[65536];
			rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
			rapidjson::Document d;
			d.ParseStream(is);
			fclose(fp);

			util::JSON json(d);

			m_fMaxSpeed = json["max_speed"].GetDouble();
			m_fGroundAcceleration = json["ground_acceleration"].GetDouble();
			m_fGroundDeceleration = json["ground_deceleration"].GetDouble();
			m_fAirAcceleration = json["air_acceleration"].GetDouble();
			m_fAirDeceleration = json["air_deceleration"].GetDouble();
			m_fJumpSpeed = json["jump_speed"].GetDouble();
			m_fJumpTime = json["jump_time"].GetDouble();
		}

		const std::string PlayerValue::GetAsString() const
		{
			std::string ret;

			ret += std::string("Max Speed:\t") + std::to_string(m_fMaxSpeed) + std::string("\n");
			ret += std::string("Grnd Accel:\t") + std::to_string(m_fGroundAcceleration) + std::string("\n");
			ret += std::string("Grnd Decel:\t") + std::to_string(m_fGroundDeceleration) + std::string("\n");
			ret += std::string("Air Accel:\t") + std::to_string(m_fAirAcceleration) + std::string("\n");
			ret += std::string("Air Decel:\t") + std::to_string(m_fAirDeceleration) + std::string("\n");
			ret += std::string("Jump Speed:\t") + std::to_string(m_fJumpSpeed) + std::string("\n");
			ret += std::string("Jump Time:\t") + std::to_string(m_fJumpTime) + std::string("\n");
			return ret;
		}


		PlayerEnt::PlayerEnt(b2Body* pBody)
			: m_pBody(pBody)
			, m_pRopeJoint(null)
			, m_pAtlas(null)
			, m_pSkelData(null)
			, m_pDrawable(null)
			, m_pGroundSensor(null)
			, m_bGrounded(false)
			, m_fJumpTime(-1.0f)
		{
			b2Fixture* pFixture = pBody->GetFixtureList();
			while (pFixture != null)
			{
				if (pFixture->IsSensor())
				{
					m_pGroundSensor = pFixture;
					break;
				}

				pFixture = pFixture->GetNext();
			}

		}

		VIRTUAL PlayerEnt::~PlayerEnt()
		{
		}

		VIRTUAL void PlayerEnt::Init()
		{
			m_tValue.LoadValues("assets/player_values.json");

			std::string file_path = std::regex_replace(path, std::regex("REP"), file);
			m_pAtlas = spine::Atlas::createFromFile((file_path + std::string(".atlas")).c_str(), null);
			spine::SkeletonJson json(*m_pAtlas);
			m_pSkelData = json.readSkeletonDataFile((file_path + std::string(".json")).c_str());
			pData = new spine::AnimationStateData(*m_pSkelData);
			m_pDrawable = new spine::SkeletonDrawable(m_pSkelData, pData);

			//m_pDrawable->state->listener = BIND5(this, &LogoEnt::AnimationListenerCallback);

			baka::render::RenderPlugin* pRenderPlug = static_cast<baka::render::RenderPlugin*>(baka::IPlugin::FindPlugin(baka::render::RenderPlugin::Type));
			pRenderPlug->AddDrawable(m_pDrawable, "physics");

			m_pDrawable->skeleton->setBonesToSetupPose();
			m_pDrawable->scale(0.004f, -0.004f);
			m_pDrawable->state->setAnimationByName(0, "idle", true);
			//pData = new spine::AnimationStateData();
			//	pData->setMixByName("walk", "idle", 5.0f);
			//m_pDrawable->state->data.defaultMix = 2.0f;

			baka::key_events::s_InputKeyDown.Subscribe(this, BIND1(this, &PlayerEnt::OnKeyDown));
			baka::key_events::s_InputKeyUp.Subscribe(this, BIND1(this, &PlayerEnt::OnKeyUp));

			baka::mouse_events::s_InputMouseButtonDown.Subscribe(this, BIND1(this, &PlayerEnt::OnMouseDown));
			baka::mouse_events::s_InputMouseButtonUp.Subscribe(this, BIND1(this, &PlayerEnt::OnMouseUp));
			baka::mouse_events::s_InputMouseMotion.Subscribe(this, BIND1(this, &PlayerEnt::OnMouseMove));

			baka::physics::contact_events::s_ContactBegin.Subscribe(this, BIND1(this, &PlayerEnt::OnContactBegin));
			baka::physics::contact_events::s_ContactEnd.Subscribe(this, BIND1(this, &PlayerEnt::OnContactEnd));

		}

		VIRTUAL void PlayerEnt::Exit()
		{
			baka::render::RenderPlugin* renderer = static_cast<baka::render::RenderPlugin*>(baka::IPlugin::FindPlugin(baka::render::RenderPlugin::Type));
			if (renderer != null)
			{
				renderer->RemDrawable(m_pDrawable);
				m_pDrawable->state->clearTracks();
				delete m_pDrawable;
			}

			delete m_pAtlas;
			delete m_pSkelData;
			delete pData;

			m_pAtlas = null;
			m_pSkelData = null;
			m_pDrawable = null;

			m_vGroundContacts.clear();
		}

		VIRTUAL void PlayerEnt::Update(const sf::Time& dt)
		{
			for (int i = 0; i < EButton::EB_COUNT; ++i)
			{
				m_vButtons[i].Flush(dt.asSeconds());
			}

			if (m_vButtons[EB_JUMP].Push() && m_bGrounded)
			{
				m_fJumpTime = m_tValue.m_fJumpTime;
				//m_pBody->ApplyLinearImpulse(b2Vec2(0, m_tValue.m_fJumpImpulse), m_pBody->GetLocalCenter(), true);

			/*	b2Vec2 jumpImpulse(0, -m_pBody->GetMass() * 10);
				for (int i = 0; i < m_vContacts.size(); ++i)
				{
					b2Fixture* pFixA = m_vContacts[i]->GetFixtureA();
					b2Fixture* pFixB = m_vContacts[i]->GetFixtureB();

					if (pFixA == m_pGroundSensor)
					{
						pFixB->GetBody()->ApplyLinearImpulse(jumpImpulse, pFixB->GetBody()->GetWorldCenter(), true);
					}
					else
					{
						pFixA->GetBody()->ApplyLinearImpulse(jumpImpulse, pFixA->GetBody()->GetWorldCenter(), true);
					}
				}*/
			}
			else if (m_vButtons[EB_JUMP].Pull())
			{
				m_fJumpTime = -1.0f;
			}
			if (m_vButtons[EB_JUMP].Held() && m_fJumpTime > 0.0f)
			{
				m_fJumpTime -= dt.asSeconds();
				float jIpulse = m_pBody->GetMass() * m_tValue.m_fJumpSpeed;
				m_pBody->ApplyLinearImpulse(b2Vec2(0, jIpulse), m_pBody->GetLocalCenter(), true);
			}


			m_pDrawable->update(dt.asSeconds());
			m_pDrawable->setPosition(m_pBody->GetPosition().x, -m_pBody->GetPosition().y);
		//	m_pDrawable->setRotation(m_pBody->GetAngle() * -RAD_DEG);

			b2Vec2 vel = m_pBody->GetLinearVelocity();
			float desiredVel = 0;
			bool hasInput = false;
			if (m_vButtons[EB_RIGHT].Held())
			{
				hasInput = true;
				if (m_bGrounded)
					desiredVel = b2Min(vel.x + m_tValue.m_fGroundAcceleration, m_tValue.m_fMaxSpeed);
				else
					desiredVel = b2Min(vel.x + m_tValue.m_fAirAcceleration, m_tValue.m_fMaxSpeed);
			}
			if (m_vButtons[EB_LEFT].Held())
			{
				hasInput = true;
				if (m_bGrounded)
					desiredVel = b2Max(vel.x - m_tValue.m_fGroundAcceleration, -m_tValue.m_fMaxSpeed);
				else
					desiredVel = b2Max(vel.x - m_tValue.m_fAirAcceleration, -m_tValue.m_fMaxSpeed);
			}
			if (!hasInput) //apply deceleration
			{
				if (m_bGrounded)
					desiredVel = vel.x * m_tValue.m_fGroundDeceleration;
				else
					desiredVel = vel.x * m_tValue.m_fAirDeceleration;
			}
			float velChange = desiredVel - vel.x;
			float impulse = m_pBody->GetMass() * velChange; //disregard time factor
			m_pBody->ApplyLinearImpulse(b2Vec2(impulse, 0), m_pBody->GetWorldCenter(), true);


			if (desiredVel > 0 )
				m_pDrawable->skeleton->flipX = false;
			else if (desiredVel < 0)
				m_pDrawable->skeleton->flipX = true;
		}


		void PlayerEnt::OnKeyDown(const baka::key_events::KeyAction& action)
		{
			switch (action.m_code)
			{
			case sf::Keyboard::Left:
			case sf::Keyboard::A:
			{
				m_vButtons[EButton::EB_LEFT].Next(true);
			}break;
			case sf::Keyboard::Right:
			case sf::Keyboard::D:
			{
				m_vButtons[EButton::EB_RIGHT].Next(true);
			}break;
			case sf::Keyboard::Up:
			case sf::Keyboard::W:
			case sf::Keyboard::Space:
			{
				m_vButtons[EButton::EB_JUMP].Next(true);
			}break;
			}
		}

		void PlayerEnt::OnKeyUp(const baka::key_events::KeyAction& action)
		{
			switch (action.m_code)
			{
			case sf::Keyboard::Left:
			case sf::Keyboard::A:
			{
				m_vButtons[EButton::EB_LEFT].Next(false);
			}break;
			case sf::Keyboard::Right:
			case sf::Keyboard::D:
			{
				m_vButtons[EButton::EB_RIGHT].Next(false);
			}break;
			case sf::Keyboard::Up:
			case sf::Keyboard::W:
			case sf::Keyboard::Space:
			{
				m_vButtons[EButton::EB_JUMP].Next(false);
			}break;
			case sf::Keyboard::R:
			{
				m_tValue.LoadValues("assets/player_values.json");
			}break;
			}
		}

		void PlayerEnt::OnMouseDown(const baka::mouse_events::ButtonAction& action)
		{
		}

		void PlayerEnt::OnMouseUp(const baka::mouse_events::ButtonAction& action)
		{
			if (action.m_button == sf::Mouse::Right)
				OnRopeEvent(action.m_pixel);
		}

		void PlayerEnt::OnMouseMove(const baka::mouse_events::MotionAction& action)
		{
		}


		void PlayerEnt::OnRopeEvent(const sf::Vector2i& screenPos)
		{
			if (m_pRopeJoint == NULL)
			{
				baka::render::RenderPlugin* pRenderPlug = static_cast<baka::render::RenderPlugin*>(baka::IPlugin::FindPlugin(baka::render::RenderPlugin::Type));
				baka::physics::PhysicsPlugin* pPhysicsPlug = static_cast<baka::physics::PhysicsPlugin*>(baka::IPlugin::FindPlugin(baka::physics::PhysicsPlugin::Type));
				sf::Vector2f worldCoords = pRenderPlug->GetRenderWindow()->mapPixelToCoords(screenPos, pPhysicsPlug->GetView());
				worldCoords.y *= -1;

				b2Vec2 pw(worldCoords.x, worldCoords.y);
				baka::physics::callbacks::RayCastClosestCallback callback;
				callback.m_pIgnore = m_pBody;
				b2Vec2 direction = pw - m_pBody->GetPosition();
				direction.Normalize();
				direction *= 100;
				b2Vec2 point = m_pBody->GetPosition() + direction;
				m_pBody->GetWorld()->RayCast(&callback, m_pBody->GetPosition() + b2Vec2(0,0.5), point);

				if (callback.m_bHit)
				{
					b2Body* body = callback.m_pFixture->GetBody();
					b2RopeJointDef md;
					md.bodyA = m_pBody;
					md.bodyB = body;
					md.localAnchorA = b2Vec2(0, 0.5);// .SetZero();
					md.localAnchorB = body->GetLocalPoint(callback.m_point);
					md.maxLength = (callback.m_point - m_pBody->GetPosition()).Length() * 1.1f;
					md.collideConnected = true;
					m_pRopeJoint = (b2RopeJoint*)m_pBody->GetWorld()->CreateJoint(&md);
					body->SetAwake(true);
				}
			}
			else
			{
				m_pBody->GetWorld()->DestroyJoint(m_pRopeJoint);
				m_pRopeJoint = NULL;
			}
		}

		void PlayerEnt::OnContactBegin(b2Contact* contact)
		{
			if (contact->GetFixtureA() != m_pGroundSensor && contact->GetFixtureB() != m_pGroundSensor)
				return;
			m_vGroundContacts.push_back(contact);

			m_bGrounded = false;
			for (int i = 0; i < m_vGroundContacts.size(); ++i)
			{
				if (m_vGroundContacts[i]->IsTouching())
				{
					m_bGrounded = true;
					return;
				}
			}
		}

		void PlayerEnt::OnContactEnd(b2Contact* contact)
		{
			for (int i = 0; i < m_vGroundContacts.size(); ++i)
			{
				if (m_vGroundContacts[i] == contact)
				{
					m_vGroundContacts.erase(m_vGroundContacts.begin() + i);
					break;
				}
			}

			m_bGrounded = false;
			for (int i = 0; i < m_vGroundContacts.size(); ++i)
			{
				if (m_vGroundContacts[i]->IsTouching())
				{
					m_bGrounded = true;
					return;
				}
			}
		}
	}
}