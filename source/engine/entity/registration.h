#pragma once
#include "entity_plugin.h"
#include "utility/helper/func.h"

namespace baka
{
	namespace entity
	{
#define REGISTER_ENTITY(CLASS)	pEntPlug->RegisterEntity(#CLASS, CLASS::Type, util::CNewType<CLASS>())

		void RegisterEngineEntities(baka::entity::EntityPlugin* pEntPlug);
	}
}