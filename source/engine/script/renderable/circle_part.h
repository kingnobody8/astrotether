#pragma once
#include "render_part.h"

namespace engine
{
	namespace script
	{
		namespace renderable
		{
			class CCirclePart : public IRenderPart
			{
			public:
				DECLARE_PART_TYPE_INFO(CCirclePart);

				/*Data*/
			private:
				util::math::vec2	m_center;
				float				m_radius;
				bool				m_fill;
			protected:

				/*Func*/
			public:
				CCirclePart();
				virtual ~CCirclePart();
				CCirclePart& operator= (const CCirclePart& that);
			
				virtual void Init();
				virtual void LoadJson(const util::JSON& json);

				virtual void OnMatrixChanged(void);
				virtual void OnZedChanged(void);

				void SetCenter(const util::math::vec2& center);
				void SetRadius(const float& radius);
				void SetFill(const bool fill);
			};
		}
	}
}