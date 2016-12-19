#pragma once
#include "SFML/Graphics.hpp"
#include "utility/event/subscriber.h"

namespace baka
{
	namespace entity
	{
		class IEntity : public util::Subscriber
		{
		protected:
			static int s_nextEntityTypeId;

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



#define DECLARE_ENTITY_TYPE_INFO(CLASS)									\
			typedef CLASS class_t;										\
			static const int Type;										\
			virtual inline int GetType() const { return Type; }			\
			virtual const char* GetTypeName() const;
#define DEFINE_ENTITY_TYPE_INFO(CLASS)									\
			const int CLASS::Type = ++baka::entity::IEntity::s_nextEntityTypeId;			\
			const char* CLASS::GetTypeName() const { return #CLASS; }
		};
	}
}