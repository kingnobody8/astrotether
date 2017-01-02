#include "entity_plugin.h"
#include "entity.h"
#include "physics_ent.h"

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
			DestroyAllEntities();
		}

		VIRTUAL bool EntityPlugin::Update(const sf::Time& dt)
		{
			for (auto iter = m_vPending.begin(); iter != m_vPending.end(); ++iter)
			{
				(*iter)->Init();
				m_vEnts.push_back(*iter);
			}
			m_vPending.clear();

			for (auto iter = m_vEnts.begin(); iter != m_vEnts.end(); ++iter)
			{
				(*iter)->Update(dt);
			}

			return true;
		}

		IEntity* EntityPlugin::CreateEntity(const std::string& type)
		{
			auto find = m_vNameMap.find(type);
			assert(find != m_vNameMap.end() && "This entity type doesn't exist!");
			IEntity* pEnt = find->second();
			m_vPending.push_back(pEnt);
			return pEnt;
		}
		IEntity* EntityPlugin::CreateEntity(const int type)
		{
			auto find = m_vKeyMap.find(type);
			assert(find != m_vKeyMap.end() && "This entity type doesn't exist!");
			IEntity* pEnt = find->second();
			m_vPending.push_back(pEnt);
			return pEnt;
		}
		void EntityPlugin::DestroyEntity(IEntity* pEnt)
		{
			for (auto iter = m_vEnts.begin(); iter != m_vEnts.end(); ++iter)
			{
				IEntity* pTemp = (*iter);
				if (pTemp == pEnt)
				{
					pEnt->Exit();
					delete pEnt;
					m_vEnts.remove((*iter));
					return;
				}
			}
		}
		void EntityPlugin::DestroyAllEntities()
		{
			for (auto iter = m_vEnts.begin(); iter != m_vEnts.end(); ++iter)
			{
				IEntity* pEnt = (*iter);
				pEnt->Exit();
				delete pEnt;
			}
			m_vEnts.clear();

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

		void EntityPlugin::CreatePhysicsEntities(b2dJson* json)
		{
			std::vector<b2Body*> vBodies;
			json->getAllBodies(vBodies);

			for (int i = 0; i < vBodies.size(); ++i)
			{
				b2Body* pBody = vBodies[i];
				if (json->hasCustomString(pBody, "entity_type"))
				{
					std::string szEntType = json->getCustomString(pBody, "entity_type");

					IPhysicsEnt* pEnt = static_cast<IPhysicsEnt*>(CreateEntity(szEntType));
					pEnt->Setup(pBody, json);
					pBody->SetUserData(pEnt);
				}
			}
		}

		std::vector<IEntity*> EntityPlugin::FindEntities(const std::string& type)
		{
			std::vector<IEntity*> ret;
			for (auto iter = m_vPending.begin(); iter != m_vPending.end(); ++iter)
			{
				if ((*iter)->GetTypeName() == type)
				{
					ret.push_back((*iter));
				}
			}
			for (auto iter = m_vEnts.begin(); iter != m_vEnts.end(); ++iter)
			{
				if ((*iter)->GetTypeName() == type)
				{
					ret.push_back((*iter));
				}
			}
			return ret;
		}


	}
}