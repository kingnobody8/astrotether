#pragma once
#include "component/part.h"

namespace engine
{
	//forward declare
	namespace render { class IRenderNode; }

	namespace script
	{
		namespace renderable
		{
			class IRenderPart : public component::IPart
			{
			public:
				/*Data*/
			protected:
				std::string m_szLayer;
				util::Color m_color;
				render::IRenderNode* m_pNode; __todo() //it may be wise to remove this and have it set on a per child class basis, because points uses an array of nodes, also because i don't think other class should access our node
				bool m_force;

				/*Func*/
			protected:
			public:
				IRenderPart(void);
				virtual ~IRenderPart(void) = 0;
				IRenderPart& operator= (const IRenderPart& that);

				virtual void Init();
				virtual void LoadJson(const util::JSON& json);

				virtual void OnZedChanged(void);
				virtual void OnVisibilityChanged(const bool visible);

				virtual const b2AABB CalcAABB(void);
				virtual inline const bool IsRenderable(void) const { return true; }

				virtual const bool IsRegistered() const;

				inline const std::string& GetLayer(void) const { return this->m_szLayer; }
				inline const util::Color GetLocalColor() const { return m_color; }
				const util::Color GetWorldColor() const;

				virtual void SetLocalColor(const util::Color& clr);
				virtual void SetLayer(const std::string& szLayer);
			};
		}
	}
}