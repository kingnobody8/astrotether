#pragma once
#include "box2d_part.h"

namespace engine
{
	namespace script
	{
		namespace box
		{
			class CFixturePart : public IBox2DPart
			{
			public:
				DECLARE_PART_TYPE_INFO(CFixturePart);

				/*Data*/
			private:
			protected:
				b2BodyDef m_bodyDef;
				b2FixtureDef m_fixtureDef;
				bool m_bSettingMatrix;
				b2Body* m_pBody;
				b2Fixture* m_pFixture;

				/*Func*/
			public:
				CFixturePart();
				virtual ~CFixturePart();
				CFixturePart& operator= (const CFixturePart& that);

				virtual void Init();
				virtual void Exit();
				virtual void LoadJson(const util::JSON& json);

				virtual void Update(const util::Time& dt);

				virtual void OnMatrixChanged();
			};
		}
	}
}