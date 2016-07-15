#pragma once
#include "render_part.h"

namespace engine
{
	namespace script
	{
		namespace renderable
		{
			//NOTE: due to the limitations of SDL render rects, the final rotation is ignored and the rect is rendered as an AABB

			class CImgPart : public IRenderPart
			{
			public:
				DECLARE_PART_TYPE_INFO(CImgPart);

				/*Data*/
			private:
				std::string m_uri;

				util::math::vec2	m_anchor;
				SDL_Texture*		m_pTexture;
				SDL_RendererFlip	m_flip;
				SDL_BlendMode		m_blend_mode;

			protected:

				/*Func*/
			public:
				CImgPart();
				virtual ~CImgPart();
				CImgPart& operator= (const CImgPart& that);
			
				virtual void Init();
				virtual void LoadJson(const util::JSON& json);
				virtual void LoadImgFromUri(const std::string& uri);

				virtual void OnMatrixChanged(void);

				inline const std::string& GetUri() const { return m_uri; }

				void SetAnchor(const util::math::vec2& anchor);
				void SetFlip(const SDL_RendererFlip& flip);
				void SetBlendMode(const SDL_BlendMode& blend_mode);


			};
		}
	}
}