#include "entity_plugin.h"
#include "entity.h"

namespace baka
{
	namespace entity
	{
		DEFINE_PLUGIN_TYPE_INFO(EntityPlugin);

		IEntity* EntityPlugin::CreateEntity(const std::string& type)
		{
			auto find = m_vNameMap.find(type);
			assert(find != m_vNameMap.end() && "This entity type doesn't exist!");
			return find->second();
		}
		IEntity* EntityPlugin::CreateEntity(const int type)
		{
			auto find = m_vKeyMap.find(type);
			assert(find != m_vKeyMap.end() && "This entity type doesn't exist!");
			return find->second();
		}
		void EntityPlugin::RegisterEntity(const std::string& typeName, const EntTypeKey& key, const EntFunctor func)
		{
			//Each part should be unique
			assert(m_vNameMap.find(typeName) == m_vNameMap.end() && "You have a duplicate entity!");
			assert(m_vKeyMap.find(key) == m_vKeyMap.end() && "You have a duplicate entity!");

			//int key = ++IPart::s_nextPartTypeId;
			m_vNameMap[typeName] = func;
			m_vKeyMap[key] = func;
		}

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

		void EntityPlugin::CreatePhysicsEntities(b2dJson* json)
		{
			std::vector<b2Body*> vBodies;
			json->getAllBodies(vBodies);

			for (int i = 0; i < vBodies.size(); ++i)
			{
				b2Body* pBody = vBodies[i];
				if (json->hasCustomString(pBody, "entity_type"))
				{

				}
			}
		}

	}
}