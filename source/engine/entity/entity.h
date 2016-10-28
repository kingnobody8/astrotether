#pragma once
#include "SFML/Graphics.hpp"


namespace baka
{
	namespace entity
	{
		class IEntity
		{
		private:
			bool m_bMarkedForDelete;

		public:
			IEntity()
				: m_bMarkedForDelete(false)
			{
			}
			virtual ~IEntity() = 0;

			virtual void Init() {}
			virtual void Exit() {}
			virtual void Update(const sf::Time& dt) {}
		};
	}
}