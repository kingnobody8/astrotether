#pragma once
#include "SFML/Graphics.hpp"
#include "engine/entity/entity.h"
#include "render/spine-sfml.h"
#include "b2djson/b2dJsonImage.h"
#include <Box2D.h>

namespace app
{
	namespace entity
	{
		class PhysicsImageEnt : public baka::entity::IEntity
		{
		private:
			b2Body* m_pBody;
			sf::Sprite* m_pSprite;
			std::string m_filepath;
			b2dJsonImage* m_pb2Image;
			sf::Vector2f imgSize;
		public:
			PhysicsImageEnt(const std::string& filepath, b2dJsonImage* pb2Image);
			virtual ~PhysicsImageEnt();

			virtual void Init();
			virtual void Exit();
			virtual void Update(const sf::Time& dt);

			sf::Sprite* GetSprite() const { return m_pSprite; }

		};
	}
}