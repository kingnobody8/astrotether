#include "render_node.h"
#include "../render_layer.h"
#include "utility/helper/func.h"

namespace engine
{
	namespace render
	{
		void IRenderNode::Register(const std::string& layer)
		{
			Unregister();

			CRenderLayer* tmp = CRenderLayer::FindLayer(layer);
			assert(tmp);

			tmp->RegisterNode(this);
			this->m_pLayer = tmp;
		}
		
		void IRenderNode::Unregister(void)
		{
			if (this->m_pLayer)
			{
				this->m_pLayer->UnregisterNode(this);
				this->m_pLayer = null;
			}
		}

		IRenderNode::IRenderNode(void)
			: m_pLayer(null)
			, m_flag(MOVE_DIRTY | CULL_DIRTY)
			, m_zed(0.0f)
			, m_vboID(0)
			, m_iboID(0)
			, m_pShader(null)
		//	, m_use_scissor(false)
		{
		}
		
		VIRTUAL IRenderNode::~IRenderNode()
		{
			Unregister();
		}
		
	/*	void IRenderNode::SetScissorRect(const b2AABB& scissor)
		{
			if (scissor.IsValid())
				return;
			m_scissor = scissor;
			m_use_scissor = true;
		}
		
		void IRenderNode::ScissorOperation(SDL_Renderer* pRen, const vec2& origin)
		{
			if (m_use_scissor)
			{
				SDL_Rect scissor;
				scissor.x = (int)(origin.x + m_scissor.lowerBound.x);
				scissor.y = (int)(origin.y - m_scissor.upperBound.y);
				scissor.w = (int)m_scissor.GetExtents().x * 2;
				scissor.h = (int)m_scissor.GetExtents().y * 2;
				SDL_RenderSetClipRect(pRen, &scissor);
			}
			else
			{
				SDL_RenderSetClipRect(pRen, null);
			}
		}
		*/
		const bool IRenderNode::CheckInView(const b2AABB& view)
		{
			__todo()
				//USE BOX2D polygonshape for shape checks, add a function to each rendernode that returns it's aabb,
				//each rendernode should then cache its aabb, only changing it if it moves
				//we can use this for fast query of aabb on parts too by asking their render nodes
				//since the nodes store the aabb, they can use it for culling
				//we probably will have to perform a full cull each frame (that means checking every node aabb against the camera aabb)
				//however one idea is to cache a bool saying if an object has moved since last cull, it has then we only check the objects that have moved
				//howeve against that idea is that if the camera moves we still have to recheck everything, so this won't be very useful during gameplay

			CalcAABB();

			const bool overlap = view.Contains(m_aabb);

			return overlap;
		}
		
		const b2AABB IRenderNode::CalcAABB()
		{
			if (m_flag.Flag(NodeStateFlag::MOVE_DIRTY))
			{
				m_flag.FlagOff(NodeStateFlag::MOVE_DIRTY);
				CalcAabbInternal();
			}

			return m_aabb;
		}
	}
}