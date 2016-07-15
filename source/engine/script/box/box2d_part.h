#pragma once
#include "script/renderable/render_part.h"
#include "Box2D/box2d.h"

namespace engine
{
	namespace script
	{
		namespace box
		{
			/*class ContactListener : public b2ContactListener
			{
				virtual void BeginContact(b2Contact* contact);

				virtual void EndContact(b2Contact* contact);

				virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);

				virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
			};*/

			class IBox2DPart : public component::IPart//script::renderable::IRenderPart
			{
			protected:
				static b2World* s_pWorld;
				static bool		s_bUpdating;
				//static ContactListener s_contactListener;

			public:
				static b2World* GetWorld() { return s_pWorld; }
				static void SetupWorld();
				static void DestroyWorld();
				static void UpdateWorld(const util::Time& dt);

				__todo() //will this be screwed up when we scale???
				static const float PIX_TO_BOX;
				static const float BOX_TO_PIX;


				/*Data*/
			private:
			protected:
				bool m_bSettingMatrix;

				/*Func*/
			public:
				IBox2DPart();
				virtual ~IBox2DPart();
				IBox2DPart& operator= (const IBox2DPart& that);

				virtual void Init();
				virtual void Exit();
				virtual void LoadJson(const util::JSON& json);

				virtual void OnMatrixChanged(void);
			};
		}
	}
}