#include "fixture_part.h"
#include "script/script.h"
#include "component/object.h"
#include "math/consts.h"

namespace engine
{
	namespace script
	{
		namespace box
		{
			DEFINE_PART_TYPE_INFO(CFixturePart);

			CFixturePart::CFixturePart()
				: m_bSettingMatrix(false)
			{
				m_priority = script::FIXTURE_PART;
			}
			VIRTUAL CFixturePart::~CFixturePart()
			{
			}
			CFixturePart& CFixturePart::operator = (const CFixturePart& that)
			{
				return *this;
			}

			VIRTUAL void CFixturePart::Init()
			{
				IBox2DPart::Init();

				util::Matrix wmat = m_pOwner->CalcWorldMatrix();

				util::vec3 pos;// = wmat.GetPosition();
				util::AABB aabb = m_pOwner->CalcAabb();
				const util::vec2 extends = util::vec2(aabb.GetExtents().x, aabb.GetExtents().y);

				const b2Vec2 b2points[4] =
				{
					b2Vec2(-extends.x * PIX_TO_BOX, -extends.y * PIX_TO_BOX),
					b2Vec2(extends.x * PIX_TO_BOX, -extends.y * PIX_TO_BOX),
					b2Vec2(extends.x * PIX_TO_BOX, extends.y * PIX_TO_BOX),
					b2Vec2(-extends.x * PIX_TO_BOX, extends.y * PIX_TO_BOX)
				};

				b2PolygonShape shape;
				shape.Set(b2points, 4);
				//shape.SetAsBox(float32(aabb.CalcExtends().x * PIX_TO_BOX), float32(aabb.CalcExtends().y * PIX_TO_BOX));

				m_bodyDef.userData = this;
				m_bodyDef.position = b2Vec2(pos.x * PIX_TO_BOX, pos.y * PIX_TO_BOX);
				//m_bodyDef.angle = wmat.GetRotation().z * DEG_TO_RAD; __todo() // there seems to be problems if the box part starts off rotated
				m_pBody = s_pWorld->CreateBody(&m_bodyDef);

				m_fixtureDef.userData = this;
				m_fixtureDef.shape = &shape;
				m_pFixture = m_pBody->CreateFixture(&m_fixtureDef);

				if (m_bodyDef.type != b2BodyType::b2_staticBody)
					EnableUpdate();
			}

			VIRTUAL void CFixturePart::Exit()
			{
				IBox2DPart::Exit();

				s_pWorld->DestroyBody(m_pBody);
				m_pBody = nullptr;
				m_pFixture = nullptr;
			}

			VIRTUAL void CFixturePart::LoadJson(const util::JSON& json)
			{
				IBox2DPart::LoadJson(json);

				assert(json.HasMember("body_def"));
				assert(json.HasMember("fixture_def"));

				const util::JSON body_def = json["body_def"];
				const util::JSON fixture_def = json["fixture_def"];

				assert(body_def.HasMember("linear_velocity"));
				assert(body_def.HasMember("angular_velocity"));
				assert(body_def.HasMember("linear_damping"));
				assert(body_def.HasMember("angular_damping"));
				assert(body_def.HasMember("allow_sleep"));
				assert(body_def.HasMember("awake"));
				assert(body_def.HasMember("fixed_rotation"));
				assert(body_def.HasMember("bullet"));
				assert(body_def.HasMember("body_type"));
				assert(body_def.HasMember("active"));
				assert(body_def.HasMember("gravity_scale"));

				assert(fixture_def.HasMember("filter"));
				assert(fixture_def.HasMember("friction"));
				assert(fixture_def.HasMember("restitution"));
				assert(fixture_def.HasMember("density"));
				assert(fixture_def.HasMember("is_sensor"));

				const util::JSON filter = fixture_def["filter"];
				assert(filter.HasMember("category_bits"));
				assert(filter.HasMember("group_index"));
				assert(filter.HasMember("mask_bits"));

				const util::vec2 linearVelocity = ((const util::JSON&)(body_def["linear_velocity"])).GetVec2();
				m_bodyDef.linearVelocity = b2Vec2(linearVelocity.x, linearVelocity.y);
				m_bodyDef.angularVelocity = body_def["angular_velocity"].GetDouble();
				m_bodyDef.linearDamping = body_def["linear_damping"].GetDouble();
				m_bodyDef.angularDamping = body_def["angular_damping"].GetDouble();
				m_bodyDef.allowSleep = body_def["allow_sleep"].GetBool();
				m_bodyDef.awake = body_def["awake"].GetBool();
				m_bodyDef.fixedRotation = body_def["fixed_rotation"].GetBool();
				m_bodyDef.bullet = body_def["bullet"].GetBool();
				m_bodyDef.type = (b2BodyType)body_def["body_type"].GetInt();
				m_bodyDef.active = body_def["active"].GetBool();
				m_bodyDef.gravityScale = body_def["gravity_scale"].GetDouble();

				m_fixtureDef.friction = fixture_def["friction"].GetDouble();
				m_fixtureDef.restitution = fixture_def["restitution"].GetDouble();
				m_fixtureDef.density = fixture_def["density"].GetDouble();
				m_fixtureDef.isSensor = fixture_def["is_sensor"].GetBool();
				m_fixtureDef.filter.categoryBits = filter["category_bits"].GetInt();
				m_fixtureDef.filter.groupIndex = filter["group_index"].GetInt();
				m_fixtureDef.filter.maskBits = filter["mask_bits"].GetInt();
			}

			VIRTUAL void CFixturePart::Update(const util::Time& dt)
			{
				b2Vec2 bpos = m_pBody->GetPosition();
				float rot = m_pBody->GetAngle() * RAD_TO_DEG;

				m_bSettingMatrix = true;
				m_pOwner->SetWorldPosXY(bpos.x * BOX_TO_PIX, bpos.y * BOX_TO_PIX);
				m_pOwner->SetWorldRotationZ(rot);
				m_bSettingMatrix = false;
			}

			VIRTUAL void CFixturePart::OnMatrixChanged()
			{
				if (m_bSettingMatrix) //we only want to respond to a Matrix changed event, if someone else changes our Matrix
					return;

				__todo()//we may want a soft assert here, because we probably should never be setting a box parts Matrix outside of the initial setup

				util::Matrix wmat = m_pOwner->CalcWorldMatrix();
				util::vec3 pos;// = wmat.GetPosition() * PIX_TO_BOX;
				//m_pBody->SetTransform(b2Vec2(pos.x, pos.y), wmat.GetRotation().z * DEG_TO_RAD);
				m_pBody->SetLinearVelocity(b2Vec2(0, 0));
				m_pBody->SetAngularVelocity(0);
				m_pBody->SetAwake(true);
			}
		}
	}
}