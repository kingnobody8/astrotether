#pragma once
#include "engine/plugin.h"
#include "b2djson/b2dJson.h"
#include "utility/helper/func.h"

namespace baka
{
	namespace entity
	{
		//forward declare
		class IEntity;

		typedef ulonglong							EntTypeKey;
		typedef std::function<IEntity*(void)>		EntFunctor;

#define CreateEntityMacro(ENT_PLUG, ENT_TYPE) static_cast<ENT_TYPE*>(ENT_PLUG->CreateEntity(ENT_TYPE::Type));

		class EntityPlugin : public IPlugin
		{
		public:
			DECLARE_PLUGIN_TYPE_INFO(EntityPlugin);
			void RegisterEntity(const std::string& typeName, const EntTypeKey& key, const EntFunctor func);

		private:
			std::map<std::string, EntFunctor> m_vNameMap;
			std::map<EntTypeKey, EntFunctor> m_vKeyMap;
			std::list<IEntity*> m_vPending;
			std::list<IEntity*> m_vEnts;

		public:
			EntityPlugin();
			virtual ~EntityPlugin();
			virtual const EPluginOrder GetPriority() const { return EPO_ENTITY; }

			virtual void Init();
			virtual void Exit();
			virtual bool Update(const sf::Time& dt);

			IEntity* CreateEntity(const std::string& type);
			IEntity* CreateEntity(const int type);
			void DestroyEntity(IEntity* pEnt);
			void DestroyAllEntities();
			void CreatePhysicsEntities(b2dJson* json);
		};
	}
}