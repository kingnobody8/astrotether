#pragma once
#include "SFML/Graphics.hpp"
#include "utility/event/subscriber.h"

namespace baka
{
	namespace entity
	{
		class IEntity : public util::Subscriber
		{
		private:
			bool m_bMarkedForDelete;

		public:
			IEntity()
				: m_bMarkedForDelete(false)
			{
			}
			virtual ~IEntity() {};

			virtual void Init() {}
			virtual void Exit() {}
			virtual void Update(const sf::Time& dt) {}
		};
	}
}