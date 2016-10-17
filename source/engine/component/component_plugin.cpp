#include "component_plugin.h"
#include "object.h"
#include "part.h"
#include "../script/script.h"

namespace baka
{
	namespace component
	{
		DEFINE_PLUGIN_TYPE_INFO(ComponentPlugin);

		ComponentPlugin::ComponentPlugin()
			: m_pRoot(null)
		{
		}

		VIRTUAL ComponentPlugin::~ComponentPlugin()
		{
		}

		VIRTUAL void ComponentPlugin::Init()
		{
			script::RegisterScripts();
		}

		VIRTUAL void ComponentPlugin::Exit()
		{
			//Destroy Scene
			if (m_pRoot)
				m_pRoot->Destroy();
			m_pRoot = null;
			component::CObject::Clean();
		}

		VIRTUAL bool ComponentPlugin::Update(const util::Time& dt)
		{
			CObject::Clean();
			IPart::UpdateParts(dt);
			CObject::Clean();

			return true;
		}
	}
}