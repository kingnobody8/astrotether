#pragma once
#include "Collision/Shapes/b2PolygonShape.h"
#include "../render_core.h"
#include "utility/types/bit_flag.h"
#include "math/math.h"
#include "utility/math/matrix.h"
#include "utility/types/color.h"
#include <string>
#include "utility/helper/macro.h"
#include "../shader/shader_program.h"

namespace engine
{
	namespace render
	{
		//Forward Declare
		class CRenderLayer;

		struct Vertex
		{
			vec2 position;
			vec2 tex_coords;
			vec4 color;

			Vertex(){}
			Vertex(vec2 pos, vec2 tcoords, vec4 clr)
				: position(pos), tex_coords(tcoords), color(clr)
			{
			}
		};

		class IRenderNode
		{
		public:
			enum NodeStateFlag { 
				MOVE_DIRTY = 0x01,
				CULL_DIRTY = 0x02,
			};

		protected:
			util::Matrix		m_matrix;
			CRenderLayer*		m_pLayer;
			b2AABB				m_aabb;
			util::Flag08		m_flag;
			util::Color			m_clr;
		/*	b2AABB				m_scissor;
			bool				m_use_scissor;*/
			float				m_zed;
			IShaderProgram*		m_pShader;

			GLuint m_vboID;
			GLuint m_iboID;

		protected:
			//void ScissorOperation(SDL_Renderer* pRen, const vec2& origin);
			virtual void CalcAabbInternal() = 0;

		public:
			IRenderNode(void);
			virtual ~IRenderNode() = 0;
			virtual void operator () (const util::Matrix& inv_cam) = 0;

			inline void SetShader(IShaderProgram* const shader) { m_pShader = shader; }
			inline IShaderProgram* const GetShader() const { return m_pShader; }

			const b2AABB CalcAABB();
			const bool CheckInView(const b2AABB& view);

			inline void SetColor(const util::Color& clr) { m_clr = clr; }
			inline void SetZed(const float& zed) { m_zed = zed; }

			inline const util::Color& GetColor() const { return m_clr; }
			inline const float& GetZed() const { return m_zed; }
			inline CRenderLayer* GetLayer() const { return m_pLayer; }

			//void SetScissorRect(const b2AABB& scissor);
			//inline void ClearScissorRect() { m_use_scissor = false; }
			//inline const b2AABB& GetScissorRect() const { return /*(m_use_scissor) ?*/ m_scissor /*: util::shape::AABB::INVALID_AABB*/; }
			//inline const bool IsScissorActive() const { return m_use_scissor; }

			void Register(const std::string& layer);
			void Unregister(void);
		};
	}
}