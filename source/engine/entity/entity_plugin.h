#pragma once
#include "engine/plugin.h"

namespace baka
{
	namespace entity
	{
		//forward declare
		class IEntity;

		class EntityPlugin : public IPlugin
		{
		public:
			DECLARE_PLUGIN_TYPE_INFO(EntityPlugin);

		private:
			std::list<IEntity*> m_vEnts;


		public:
			EntityPlugin();
			virtual ~EntityPlugin();
			virtual const EPluginOrder GetPriority() const { return EPO_ENTITY; }

			virtual void Init();
			virtual void Exit();
			virtual bool Update(const sf::Time& dt);
		};
	}
}