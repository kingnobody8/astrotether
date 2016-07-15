#include "img_part.h"
#include "../script.h"
#include "render/node/sprite_node.h"
#include "component/object.h"
#include "asset/loader.h"
#include "asset/resource_path.h"

namespace engine
{
	namespace script
	{
		namespace renderable
		{
			DEFINE_PART_TYPE_INFO(CImgPart);

			CImgPart::CImgPart()
			{
				m_priority = EPriorities::IMG_PART;
				m_pNode = new render::CRenderNodeSprite();
			}
			VIRTUAL CImgPart::~CImgPart()
			{
				delete m_pNode;
			}
			CImgPart& CImgPart::operator = (const CImgPart& that)
			{
				IRenderPart::operator=(that);
				m_uri = that.m_uri;
				m_pTexture = that.m_pTexture;
				m_flip = that.m_flip;
				m_blend_mode = that.m_blend_mode;
				return *this;
			}

			VIRTUAL void CImgPart::Init()
			{
				IRenderPart::Init();

				m_force = true;
				LoadImgFromUri(m_uri);
				SetAnchor(m_anchor);
				SetBlendMode(m_blend_mode);
				SetFlip(m_flip);
				m_force = false;
			}

			VIRTUAL void CImgPart::LoadJson(const util::JSON& json)
			{
				IRenderPart::LoadJson(json);
				
				assert(json.HasMember("uri"));
				assert(json.HasMember("blend_mode"));
				assert(json.HasMember("flip"));
				assert(json.HasMember("anchor"));

				m_uri = json["uri"].GetString();
				m_blend_mode = (SDL_BlendMode)json["blend_mode"].GetInt();
				m_flip = (SDL_RendererFlip)json["flip"].GetInt();
				m_anchor = ((const util::JSON&)(json["anchor"])).GetVec2();

				assert(!m_uri.empty());
			}

			VIRTUAL void CImgPart::LoadImgFromUri(const std::string& uri)
			{
				if (!m_force && (m_uri == uri))
					return;

				m_uri = uri;
				SDL_Texture* tex = asset::FileLoaderSdlTexture(getResourcePath() + uri);
				static_cast<render::CRenderNodeSprite*>(m_pNode)->SetTexture(tex);
			}

			VIRTUAL void CImgPart::OnMatrixChanged(void)
			{
				const util::math::Matrix2D wmat = m_pOwner->CalcWorldMatrix();
				render::CRenderNodeSprite* sprite = static_cast<render::CRenderNodeSprite*>(m_pNode);
				sprite->SetMatrix(wmat);
			}

			void CImgPart::SetAnchor(const util::math::vec2& anchor)
			{
				if (!m_force && (m_anchor == anchor))
					return;

				m_anchor = anchor;
				render::CRenderNodeSprite* sprite = static_cast<render::CRenderNodeSprite*>(m_pNode);
				sprite->SetAnchor(anchor);
			}

			void CImgPart::SetFlip(const SDL_RendererFlip& flip)
			{
				if (!m_force && (m_flip == flip))
					return;

				m_flip = flip;
				render::CRenderNodeSprite* sprite = static_cast<render::CRenderNodeSprite*>(m_pNode);
				sprite->SetFlip(flip);
			}
			
			void CImgPart::SetBlendMode(const SDL_BlendMode& blend_mode)
			{
				if (!m_force && (m_blend_mode == blend_mode))
					return;

				m_blend_mode = blend_mode;
				render::CRenderNodeSprite* sprite = static_cast<render::CRenderNodeSprite*>(m_pNode);
				sprite->SetBlendMode(blend_mode);
			}
		}
	}
}