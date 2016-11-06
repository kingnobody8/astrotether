#pragma once
#include "Box2D.h"
#include "b2djson/b2dJsonImage.h"
#include "SFML/Graphics.hpp"

namespace baka
{
	namespace render
	{
		class PhysicsDrawable : public sf::Drawable, public sf::Transformable {
		public:
			sf::VertexArray* m_vVertexArray;
			sf::Texture* m_pTexture;

			PhysicsDrawable(const b2dJsonImage& jImage, const std::string& path);
			~PhysicsDrawable();

			virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		private:
			b2dJsonImage m_jImage;
			std::string m_path;
			b2Body* m_pBody;
			sf::Transform m_offsetTransform;
		};
	}
} 
