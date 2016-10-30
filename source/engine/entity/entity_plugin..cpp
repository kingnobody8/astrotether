#include "entity_plugin.h"
#include "entity.h"

namespace baka
{
	namespace entity
	{
		DEFINE_PLUGIN_TYPE_INFO(EntityPlugin);

		EntityPlugin::EntityPlugin()
		{
			m_bDebugDraw = false;
		}

		VIRTUAL EntityPlugin::~EntityPlugin()
		{
		}

		VIRTUAL void EntityPlugin::Init()
		{
		}

		VIRTUAL void EntityPlugin::Exit()
		{
		}

		VIRTUAL bool EntityPlugin::Update(const sf::Time& dt)
		{
			for (auto iter = m_vEnts.begin(); iter != m_vEnts.end(); ++iter)
			{

			}

			return true;
		}

	}
}