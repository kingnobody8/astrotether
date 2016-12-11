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
#include "tether_ent.h"

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
			m_fFlipTime = json["flip_time"].GetDouble();
			m_fDashImpulse = json["dash_impulse"].GetDouble();
			m_fTetherLength = json["tether_length"].GetDouble();
			m_fTetherAngle = json["tether_angle"].GetDouble();
			m_fTongueTipRadius = json["tongue_tip_radius"].GetDouble();
			m_fTongueTipDensity = json["tongue_tip_density"].GetDouble();
			m_fTongueTipSpeed = json["tongue_tip_speed"].GetDouble();
		}

		const std::string PlayerValue::GetAsString() const
		{
			std::string ret;

			//ret += std::string("Max Speed:\t") + std::to_string(m_fMaxSpeed) + std::string("\n");
			//ret += std::string("Grnd Accel:\t") + std::to_string(m_fGroundAcceleration) + std::string("\n");
			//ret += std::string("Grnd Decel:\t") + std::to_string(m_fGroundDeceleration) + std::string("\n");
			//ret += std::string("Air Accel:\t") + std::to_string(m_fAirAcceleration) + std::string("\n");
			//ret += std::string("Air Decel:\t") + std::to_string(m_fAirDeceleration) + std::string("\n");
			//ret += std::string("Jump Speed:\t") + std::to_string(m_fJumpSpeed) + std::string("\n");
			//ret += std::string("Jump Time:\t") + std::to_string(m_fJumpTime) + std::string("\n");
			//ret += std::string("Flip Time:\t") + std::to_string(m_fFlipTime) + std::string("\n");
			//ret += std::string("Dash Imp:\t") + std::to_string(m_fDashImpulse) + std::string("\n");
			//ret += std::string("Tether Lng:\t") + std::to_string(m_fTetherLength) + std::string("\n");
			//ret += std::string("Tether Angle:\t") + std::to_string(m_fTetherAngle) + std::string("\n");
			ret += std::string("Tip Radius:\t") + std::to_string(m_fTongueTipRadius) + std::string("\n");
			ret += std::string("Tip Density:\t") + std::to_string(m_fTongueTipDensity) + std::string("\n");
			ret += std::string("Tip Speed:\t") + std::to_string(m_fTongueTipSpeed) + std::string("\n");
			return ret;
		}


		PlayerEnt::PlayerEnt(b2Body* pBody)
			: m_pBody(pBody)
			, m_pTetherEnt(null)
			, m_pTongueTip(null)
			, m_pRopeJoint(null)
			, m_pAtlas(null)
			, m_pSkelData(null)
			, m_pDrawable(null)
			, m_pGroundSensor(null)
			, m_bGrounded(false)
			, m_fJumpTime(-1.0f)
			, m_pTongueContactUse(null)
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

			baka::joypad_events::s_InputJoypadButtonDown.Subscribe(this, BIND1(this, &PlayerEnt::OnJoypadButtonDown));
			baka::joypad_events::s_InputJoypadButtonUp.Subscribe(this, BIND1(this, &PlayerEnt::OnJoypadButtonUp));
			baka::joypad_events::s_InputJoypadMove.Subscribe(this, BIND1(this, &PlayerEnt::OnJoypadMove));

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

			DestroyChain();
		}

		VIRTUAL void PlayerEnt::Update(const sf::Time& dt)
		{
			if (m_pTetherEnt)
			{
				m_pTetherEnt->Update(dt); __todo() //i need an entity system so i don't have to manually update all the entities
			}

			for (int i = 0; i < EButton::EB_COUNT; ++i)
			{
				m_vButtons[i].Flush(dt.asSeconds());
			}

			if (m_vButtons[EB_SHOOT].Pull())
			{
				Shoot();
			}
			if (m_vButtons[EB_DASH].Pull())
			{
				Dash();
			}

			if (m_vButtons[EB_LEFT_FLIP].Pull())
			{
				m_fFlipTime = m_tValue.m_fFlipTime;
			}

			if (m_fFlipTime > 0)
			{
				float desiredAngle = 0;
				m_fFlipTime -= dt.asSeconds();
				if (m_fFlipTime <= 0)
				{
					float desiredAngle = 0;
					m_pBody->SetTransform(m_pBody->GetPosition(), 0);
					m_pBody->SetFixedRotation(true);
				}
				else
				{
					desiredAngle = 180 * DEG_RAD;// Lerp(0, 360, 1.0f - (m_fFlipTime / m_tValue.m_fFlipTime));

					float bodyAngle = m_pBody->GetAngle();
					float nextAngle = bodyAngle + m_pBody->GetAngularVelocity() / 60.0;
					float totalRotation = desiredAngle - nextAngle;
					while (totalRotation < -180 * DEG_RAD) totalRotation += 360 * DEG_RAD;
					while (totalRotation > 180 * DEG_RAD) totalRotation -= 360 * DEG_RAD;
					float desiredAngularVelocity = totalRotation * 60;
					float change = 10 * DEG_RAD; //allow 1 degree rotation per time step
					desiredAngularVelocity = Min(change, Max(-change, desiredAngularVelocity));
					float impulse = m_pBody->GetInertia() * desiredAngularVelocity;
					m_pBody->SetFixedRotation(false);
					m_pBody->ApplyAngularImpulse(impulse, true);
				}
			}

			if (m_vButtons[EB_JUMP].Push() && m_bGrounded)
			{
				m_fJumpTime = m_tValue.m_fJumpTime;
				//m_pBody->ApplyLinearImpulse(b2Vec2(0, m_tValue.m_fJumpImpulse), m_pBody->GetLocalCenter(), true);

				//b2Vec2 jumpImpulse(0, -m_pBody->GetMass() * 10);
				//for (int i = 0; i < m_vGroundContacts.size(); ++i)
				//{
				//	b2Fixture* pFixA = m_vGroundContacts[i]->GetFixtureA();
				//	b2Fixture* pFixB = m_vGroundContacts[i]->GetFixtureB();
				//
				//	if (pFixA == m_pGroundSensor)
				//	{
				//		pFixB->GetBody()->ApplyLinearImpulse(jumpImpulse, pFixB->GetBody()->GetWorldCenter(), true);
				//	}
				//	else
				//	{
				//		pFixA->GetBody()->ApplyLinearImpulse(jumpImpulse, pFixA->GetBody()->GetWorldCenter(), true);
				//	}
				//}
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

			if (m_vButtons[EB_RIGHT].Held())
				m_pDrawable->skeleton->flipX = false;
			else if (m_vButtons[EB_LEFT].Held())
				m_pDrawable->skeleton->flipX = true;

			if (m_pTongueContactUse)
			{
				//b2Vec2 vel = m_pTongueTip->GetLinearVelocity();
				//b2Vec2 pnt = m_pTongueTip->GetPosition();// +vel.Normalize() * m_tValue.m_fTongueTipRadius;

				b2Vec2 pnt;
				b2WorldManifold mani;
				m_pTongueContactUse->GetWorldManifold(&mani);
				b2Body* otherBody;
				if (m_pTongueContactUse->GetFixtureA()->GetBody() == m_pTongueTip)
				{
					otherBody = m_pTongueContactUse->GetFixtureB()->GetBody();
					pnt = otherBody->GetWorldPoint(m_TongueLocalCoords);// mani.points[0];
				}
				else
				{
					otherBody = m_pTongueContactUse->GetFixtureA()->GetBody();
					pnt = otherBody->GetWorldPoint(m_TongueLocalCoords);// mani.points[0];
				}



				MakeRopeJoint(otherBody, pnt);
				m_pBody->GetWorld()->DestroyBody(m_pTongueTip);
				m_pTongueTip = null;

				m_pTongueContactUse = null;
			}
		}


		void PlayerEnt::OnKeyDown(const baka::key_events::KeyAction& action)
		{
			switch (action.m_code)
			{
			case sf::Keyboard::Q:
			{
				m_vButtons[EButton::EB_LEFT_FLIP].Next(true);
			}break;
			case sf::Keyboard::E:
			{
				m_vButtons[EButton::EB_RIGHT_FLIP].Next(true);
			}	break;
			case sf::Keyboard::J:
			{
				m_vButtons[EButton::EB_SHOOT].Next(true);
			}break;
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
			{
				m_vButtons[EButton::EB_UP].Next(true);
			}break;
			case sf::Keyboard::Down:
			case sf::Keyboard::S:
			{
				m_vButtons[EButton::EB_DOWN].Next(true);
			}break;
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
			case sf::Keyboard::Q:
			{
				m_vButtons[EButton::EB_LEFT_FLIP].Next(false);
			}break;
			case sf::Keyboard::E:
			{
				m_vButtons[EButton::EB_RIGHT_FLIP].Next(false);
			}	break;
			case sf::Keyboard::J:
			{
				m_vButtons[EButton::EB_SHOOT].Next(false);
			}break;
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
			{
				m_vButtons[EButton::EB_UP].Next(false);
			}break;
			case sf::Keyboard::Down:
			case sf::Keyboard::S:
			{
				m_vButtons[EButton::EB_DOWN].Next(false);
			}break;
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
			{
				if (DestroyChain())
					return;
				baka::render::RenderPlugin* pRenderPlug = static_cast<baka::render::RenderPlugin*>(baka::IPlugin::FindPlugin(baka::render::RenderPlugin::Type));
				baka::physics::PhysicsPlugin* pPhysicsPlug = static_cast<baka::physics::PhysicsPlugin*>(baka::IPlugin::FindPlugin(baka::physics::PhysicsPlugin::Type));
				sf::Vector2f worldCoords = pRenderPlug->GetRenderWindow()->mapPixelToCoords(action.m_pixel, pPhysicsPlug->GetView());
				worldCoords.y *= -1;
				OnRopeEvent(worldCoords);
			}
		}

		void PlayerEnt::OnMouseMove(const baka::mouse_events::MotionAction& action)
		{
		}

		void PlayerEnt::OnJoypadButtonDown(const baka::joypad_events::ButtonAction& action)
		{
			if (action.m_id > 1)
				return;

			switch (action.m_code)
			{
			case 0:
				m_vButtons[EB_SHOOT].Next(true); break;
			case 1:
				m_vButtons[EB_JUMP].Next(true); break;
			case 3:
				m_vButtons[EB_DASH].Next(true); break;
			}
		}

		void PlayerEnt::OnJoypadButtonUp(const baka::joypad_events::ButtonAction& action)
		{
			if (action.m_id > 1)
				return;

			switch (action.m_code)
			{
			case 0:
				m_vButtons[EB_SHOOT].Next(false); break;
			case 1:
				m_vButtons[EB_JUMP].Next(false); break;
			case 3:
				m_vButtons[EB_DASH].Next(false); break;
			}
		}

		void PlayerEnt::OnJoypadMove(const baka::joypad_events::AxisAction& action)
		{
			__todo()//fix movement between controller and keyboard overlapping eachotehr
				//fix dpad not registering buttons
				if (action.m_id > 1)
					return;

			if (action.m_axis != sf::Joystick::Axis::PovX && action.m_axis != sf::Joystick::Axis::PovY)
				return;

			switch (action.m_axis)
			{
			case sf::Joystick::Axis::PovX:
			{
				if (EPSI(action.m_pos, 0))
				{
					m_vButtons[EB_RIGHT].Next(false);
					m_vButtons[EB_LEFT].Next(false);
				}
				else if (action.m_pos > 0)
				{
					m_vButtons[EB_RIGHT].Next(true);
				}
				else if (action.m_pos < 0)
				{
					m_vButtons[EB_LEFT].Next(true);
				}
			} break;
			case sf::Joystick::Axis::PovY:
			{
				if (EPSI(action.m_pos, 0))
				{
					m_vButtons[EB_UP].Next(false);
					m_vButtons[EB_DOWN].Next(false);
				}
				else if (action.m_pos > 0)
				{
					m_vButtons[EB_UP].Next(true);
				}
				else if (action.m_pos < 0)
				{
					m_vButtons[EB_DOWN].Next(true);
				}

			} break;
			}
		}

		const b2Vec2 PlayerEnt::CalcShootDirection() const
		{
			b2Vec2 dir(0, 0);

			if (m_vButtons[EB_LEFT].Held())
				dir.x -= 1;
			if (m_vButtons[EB_RIGHT].Held())
				dir.x += 1;
			if (m_vButtons[EB_DOWN].Held())
				dir.y -= 1;
			if (m_vButtons[EB_UP].Held())
				dir.y += 1;

			if (dir.x == 0.0f && !m_vButtons[EB_DOWN].Held() && !m_vButtons[EB_UP].Held())
			{
				if (m_pDrawable->skeleton->flipX)
					dir.x = -1;
				else
					dir.x = 1;
			}

			dir.Normalize();
			return dir;
		}



		bool PlayerEnt::OnRopeEvent(const sf::Vector2f& worldCoords)
		{
			b2Vec2 pw(worldCoords.x, worldCoords.y);
			baka::physics::callbacks::RayCastClosestCallback callback;
			callback.m_pIgnore = m_pBody;
			b2Vec2 direction = pw - m_pBody->GetPosition();
			direction.Normalize();
			direction *= m_tValue.m_fTetherLength;
			b2Vec2 point = m_pBody->GetPosition() + b2Vec2(0, 0.5) + direction;
			m_pBody->GetWorld()->RayCast(&callback, m_pBody->GetPosition() + b2Vec2(0, 0.5), point);

			if (callback.m_bHit)
			{
				MakeRopeJoint(callback.m_pFixture->GetBody(), callback.m_point);
			}

			return callback.m_bHit;
		}

		void PlayerEnt::MakeRopeJoint(b2Body* pBody, b2Vec2 worldPoint)
		{
			float len = (worldPoint - m_pBody->GetPosition()).Length() * 1.01f;
			if (len > m_tValue.m_fTetherLength)
				return;

			m_pTetherEnt = new TetherEnt(m_pBody, pBody,m_pBody->GetPosition(), worldPoint);
			m_pTetherEnt->Init();
			return;


			b2Vec2 dir = m_pBody->GetPosition() - worldPoint;
			dir.Normalize();
			const b2Vec2 linkSize(0.1f, 0.05f);

			if (len > m_tValue.m_fTetherLength)
				return;

			b2RopeJointDef md;
			md.bodyA = m_pBody;
			md.bodyB = pBody;
			md.localAnchorA = b2Vec2(0, 0.5);// .SetZero();
			md.localAnchorB = pBody->GetLocalPoint(worldPoint);
			md.maxLength = len;
			md.collideConnected = true;
			m_pRopeJoint = (b2RopeJoint*)m_pBody->GetWorld()->CreateJoint(&md);
			pBody->SetAwake(true);

			return;


			b2RevoluteJointDef jd;
			jd.collideConnected = false;
			jd.enableLimit = true;
			jd.upperAngle = (90 * RAD_DEG);

			const int32 N = len / (linkSize.x * 2) - 1;
			b2PolygonShape shape;
			shape.SetAsBox(linkSize.x, linkSize.y);
			b2FixtureDef fd;
			fd.shape = &shape;
			fd.density = 1.0f;
			//fd.friction = 0.2f;
			fd.filter.categoryBits = 0x0001;
			fd.filter.maskBits = 0xFFFF & ~0x0002;


			b2Body* prevBody = pBody;
			for (int32 i = 0; i < N; ++i)
			{
				b2BodyDef bd;
				bd.type = b2_dynamicBody;
				b2Vec2 pos(worldPoint.x + linkSize.x * (i + 1) * 2, worldPoint.y);
				//b2Vec2 pos(worldPoint.x + dir.x * linkSize.x * i, worldPoint.y + dir.y * linkSize.y * i);
				//pos.x = worldPoint.x + dir.x * linkSize.x + dir.x * linkSize.x * i * 2;
				//pos.y = worldPoint.y + dir.y * linkSize.y + dir.y * linkSize.y * i * 2;
				bd.position.Set(pos.x, pos.y);
				//bd.angle = atan2(dir.x, dir.y) * RAD_DEG;
				//if (i == N - 1)
				//{
				//	//b2Vec2 anchor(pos.x + linkSize.x, pos.y);
				//	jd.Initialize(prevBody, m_pBody, m_pBody->GetWorldPoint(md.localAnchorA));
				//	m_pBody->GetWorld()->CreateJoint(&jd);
				//
				//	prevBody = m_pBody;
				//	break;
				//}

				b2Body* body = m_pBody->GetWorld()->CreateBody(&bd);
				m_vChain.push_back(body);

				body->CreateFixture(&fd);

				b2Vec2 anchor(pos.x - linkSize.x, pos.y);
				jd.Initialize(prevBody, body, anchor);
				m_pBody->GetWorld()->CreateJoint(&jd);

				prevBody = body;
			}
		}

		bool PlayerEnt::DestroyChain(void)
		{
			if (m_pTetherEnt)
			{
				m_pTetherEnt->Exit();
				delete m_pTetherEnt;
				m_pTetherEnt = null;
				return true;
			}
			return false;


			if (!m_pRopeJoint)
				return false;
			m_pBody->GetWorld()->DestroyJoint(m_pRopeJoint);
			m_pRopeJoint = NULL;
			//for (int i = 0; i < m_vChain.size(); ++i)
			//{
			//	m_pBody->GetWorld()->DestroyBody(m_vChain[i]);
			//}
			//m_vChain.clear();
			return true;
		}

		void PlayerEnt::Shoot()
		{
			if (DestroyChain())
				return;

			const b2Vec2 dir = CalcShootDirection();
			const b2Vec2 pos = m_pBody->GetPosition();
			b2Vec2 endPoint = pos + dir * m_tValue.m_fTetherLength;

			if (m_pTongueTip != null)
			{
				m_pTongueTip->GetWorld()->DestroyBody(m_pTongueTip);
				m_pTongueTip = null;
			}

			//body definition
			b2BodyDef def;
			def.type = b2_dynamicBody;
			def.linearVelocity = dir * m_tValue.m_fTongueTipSpeed;
			def.bullet = true;

			//shape definition
			b2CircleShape circleShape;
			circleShape.m_radius = m_tValue.m_fTongueTipRadius;

			//fixture definition
			b2FixtureDef myFixtureDef;
			myFixtureDef.shape = &circleShape;
			myFixtureDef.density = m_tValue.m_fTongueTipDensity;

			//create dynamic body
			b2Vec2 startPoint = pos + b2Vec2(dir.x * 2, dir.y * 3);
			def.position.Set(startPoint.x, startPoint.y);
			m_pTongueTip = m_pBody->GetWorld()->CreateBody(&def);
			m_pTongueTip->CreateFixture(&myFixtureDef);

			/*bool hit = OnRopeEvent(sf::Vector2f(endPoint.x, endPoint.y));

			if (!hit)
			{
			float angle = atan2(dir.y, dir.x);
			b2Vec2 tmp;
			tmp.x = cos(angle + m_tValue.m_fTetherAngle * DEG_RAD);
			tmp.y = sin(angle + m_tValue.m_fTetherAngle * DEG_RAD);
			tmp.Normalize();
			endPoint = pos + tmp * m_tValue.m_fTetherLength;
			hit = OnRopeEvent(sf::Vector2f(endPoint.x, endPoint.y));
			}
			if (!hit)
			{
			float angle = atan2(dir.y, dir.x);
			b2Vec2 tmp;
			tmp.x = cos(angle - m_tValue.m_fTetherAngle * DEG_RAD);
			tmp.y = sin(angle - m_tValue.m_fTetherAngle * DEG_RAD);
			tmp.Normalize();
			endPoint = pos + tmp * m_tValue.m_fTetherLength;
			hit = OnRopeEvent(sf::Vector2f(endPoint.x, endPoint.y));
			}*/
		}

		void PlayerEnt::Dash()
		{
			b2Vec2 dir = CalcShootDirection();
			//b2Vec2 vel = m_pBody->GetLinearVelocity();
			//float velChange = m_tValue.m_fDashImpulse - vel.x;
			//float impulse = m_pBody->GetMass() * velChange;
			dir *= m_pBody->GetMass() * m_tValue.m_fDashImpulse;
			m_pBody->ApplyLinearImpulse(dir, m_pBody->GetLocalCenter(), true);
		}

		void PlayerEnt::OnContactBegin(b2Contact* contact)
		{
			if (contact->GetFixtureA() == m_pGroundSensor || contact->GetFixtureB() == m_pGroundSensor)
			{
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
			else if (contact->GetFixtureA()->GetBody() == m_pTongueTip || contact->GetFixtureB()->GetBody() == m_pTongueTip)
			{
				m_vTongueContacts.push_back(contact);
				if (contact->IsTouching())
				{
					m_pTongueContactUse = contact;

					b2Vec2 pnt;
					b2WorldManifold mani;
					m_pTongueContactUse->GetWorldManifold(&mani);
					b2Body* otherBody = null;
					if (m_pTongueContactUse->GetFixtureA()->GetBody() == m_pTongueTip)
					{
						otherBody = m_pTongueContactUse->GetFixtureB()->GetBody();
						//	m_TongueLocalCoords = mani.points[0];
						m_TongueLocalCoords = otherBody->GetLocalPoint(mani.points[0]);
					}
					else
					{
						otherBody = m_pTongueContactUse->GetFixtureA()->GetBody();
						m_TongueLocalCoords = otherBody->GetLocalPoint(mani.points[0]);
					}

					//OnRopeEvent(sf::Vector2f(pnt.x, pnt.y));
				}
			}
		}

		void PlayerEnt::OnContactEnd(b2Contact* contact)
		{
			for (int i = 0; i < m_vTongueContacts.size(); ++i)
			{
				if (m_vTongueContacts[i] == contact)
				{
					m_vTongueContacts.erase(m_vTongueContacts.begin() + i);
					break;
				}
			}

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