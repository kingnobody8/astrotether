#pragma once
#include "render_part.h"

namespace engine
{
	namespace script
	{
		namespace renderable
		{
			class CPointsPart : public IRenderPart
			{
			public:
				DECLARE_PART_TYPE_INFO(CPointsPart);

				enum ERenderMode {
					INVALID = -1,
					POINTS,
					OPEN_LINE,
					CLOSED_LINE,
					COUNT
				};

				/*Data*/
			private:
				std::vector<util::math::vec2> m_points;
				std::vector<render::IRenderNode*> m_nodes;
				ERenderMode m_render_mode;
			protected:

				/*Func*/
			private:
				void OnMatrixChangedPoints();
				void OnMatrixChangedLines();

				void ClearNodes();
				void SetupNodes();
				void SetupPoints();
				void SetupLines(const int& count);
			public:
				CPointsPart();
				virtual ~CPointsPart();
				CPointsPart& operator= (const CPointsPart& that);
			
				virtual void Init();
				virtual void LoadJson(const util::JSON& json);

				virtual void OnZedChanged();
				virtual void OnMatrixChanged();
				virtual void OnVisibilityChanged(const bool visible);

				virtual const b2AABB CalcAABB(void);

				const bool IsRegistered() const;

				void SetLocalColor(const util::Color& clr);
				void SetLayer(const std::string& szLayer);

				void SetPointsAndRenderMode(const std::vector<util::math::vec2>& points, ERenderMode mode);

				inline const std::vector<util::math::vec2>& GetPoints() const { return m_points; }
				inline const ERenderMode& GetRenderMode() const { return m_render_mode; }
				const std::vector<util::math::vec2> CalcWorldPoints() const;
			};
		}
	}
}