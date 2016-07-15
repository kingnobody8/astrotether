#pragma once
#include "plugin.h"
#include "component.h"

namespace engine
{
	namespace component
	{
		class ComponentPlugin : public IPlugin
		{
		public:
			DECLARE_PLUGIN_TYPE_INFO(ComponentPlugin);

		private:
			CObject* m_pRoot;

		private:

		public:
			ComponentPlugin();
			virtual ~ComponentPlugin();

			virtual void Init();
			virtual void Exit();
			virtual bool Update(const util::Time& dt);

			void SetRoot(CObject* const pRoot) { m_pRoot = pRoot; }
			inline CObject* GetRoot() const { return m_pRoot; }
		};
	}
}