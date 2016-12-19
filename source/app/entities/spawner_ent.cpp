#pragma once
#include "spawner_ent.h"

namespace app
{
	namespace entity
	{
		DEFINE_ENTITY_TYPE_INFO(SpawnerEnt);

		SpawnerEnt::SpawnerEnt()
		{
		}

		VIRTUAL void SpawnerEnt::Setup(b2Body* pBody, b2dJson* pJson)
		{
		}

		VIRTUAL void SpawnerEnt::Init()
		{
		}

		VIRTUAL void SpawnerEnt::Exit()
		{
		}

		VIRTUAL void SpawnerEnt::Update(const sf::Time& dt)
		{
		}
	}
}