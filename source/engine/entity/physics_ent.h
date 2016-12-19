#pragma once
#include "entity.h"
#include "b2djson/b2dJson.h"

namespace baka
{
	namespace entity
	{
		__todo()// maybe change the loading of 'physics ent' to the physics plugin not the entity plugin
			//also maybe we should just make loader -> object functions for non physics objects

		class IPhysicsEnt : public IEntity
		{
		public:
			IPhysicsEnt() {}
			virtual void Setup(b2Body* pBody, b2dJson* pJson) {}
		};
	}
}