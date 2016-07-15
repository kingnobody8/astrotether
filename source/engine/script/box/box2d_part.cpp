#include "box2d_part.h"
#include "script/script.h"
#include "component/object.h"

namespace engine
{
	namespace script
	{
		namespace box
		{
			const int VELOCITY_ITERATIONS = 8;
			const int POSITION_ITERATIONS = 3;

			STATIC const float IBox2DPart::PIX_TO_BOX = 0.015625f;
			STATIC const float IBox2DPart::BOX_TO_PIX = 64.0f;

			STATIC b2World* IBox2DPart::s_pWorld = null;
			STATIC bool		IBox2DPart::s_bUpdating = false;
			//STATIC ContactListener IBox2DPart::s_contactListener;


			STATIC void IBox2DPart::SetupWorld()
			{
				b2Vec2 gravity;
				gravity.Set(0.0f, -10.0f);
				s_pWorld = new b2World(gravity);
				//s_pWorld->SetContactListener(&s_contactListener);
			}

			STATIC void IBox2DPart::DestroyWorld()
			{
				__todo() //remove all the box2d parts here???
					SafeDelete(s_pWorld);
			}

			STATIC void IBox2DPart::UpdateWorld(const util::Time& dt)
			{
				s_bUpdating = true;
				s_pWorld->SetAllowSleeping(true);
				s_pWorld->SetWarmStarting(true);
				s_pWorld->SetContinuousPhysics(true);
				s_pWorld->SetSubStepping(false);

				double delta = dt.ToDouble() / 1000;
				s_pWorld->Step(delta, VELOCITY_ITERATIONS, POSITION_ITERATIONS);
				s_bUpdating = false;
			}

			IBox2DPart::IBox2DPart()
				: m_bSettingMatrix(false)
			{
			}
			VIRTUAL IBox2DPart::~IBox2DPart()
			{
			}
			IBox2DPart& IBox2DPart::operator = (const IBox2DPart& that)
			{
				return *this;
			}

			VIRTUAL void IBox2DPart::Init()
			{
			}

			VIRTUAL void IBox2DPart::Exit()
			{
				assert(!s_bUpdating && "You shouldn't destroy a box part while the box2d world is being updated");
			}

			VIRTUAL void IBox2DPart::LoadJson(const util::JSON& json)
			{
				__todo() //NOTE we don't want to call renderpart's load because we don't need the color or layer (we should set the color based on what type (dynamic, static, sleeping, active) and the layer should be the debug layer)
				//IRenderPart::LoadJson(json);
			}

			VIRTUAL void IBox2DPart::OnMatrixChanged(void)
			{
				if (m_bSettingMatrix) //we only want to respond to a Matrix changed event, if someone else changes our Matrix
					return;

					
			}

		}
	}
}