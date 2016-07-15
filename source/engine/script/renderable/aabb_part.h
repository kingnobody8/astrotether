#pragma once
#include "render_part.h"

namespace engine
{
	namespace script
	{
		namespace renderable
		{
			//NOTE: due to the limitations of SDL render rects, the final rotation is ignored and the rect is rendered as an AABB

			class CAabbPart : public IRenderPart
			{
			public:
				DECLARE_PART_TYPE_INFO(CAabbPart);

				/*Data*/
			private:
				util::math::vec2 m_extends;
			protected:

				/*Func*/
			public:
				CAabbPart();
				virtual ~CAabbPart();
				CAabbPart& operator= (const CAabbPart& that);
			
				virtual void Init();
				virtual void LoadJson(const util::JSON& json);

				virtual void OnMatrixChanged(void);
				virtual void OnZedChanged(void);

				void SetExtends(const util::math::vec2& extends);
			};
		}
	}
}