#include "plugin.h"
#include <assert.h>

namespace engine
{
	STATIC std::list<IPlugin*> IPlugin::s_pluginList;

	STATIC int IPlugin::s_nextPluginTypeId = 0;

	STATIC void IPlugin::AddPlugin(IPlugin* const plugin)
	{
		assert(!FindPlugin(plugin->GetType()));
		plugin->Init();
		s_pluginList.push_back(plugin);
	}

	STATIC IPlugin* IPlugin::FindPlugin(const int& type)
	{
		for (auto iter = s_pluginList.begin(); iter != s_pluginList.end(); ++iter)
		{
			if ((*iter)->GetType() == type)
			{
				return (*iter);
			}
		}
		return null;
	}

	STATIC bool IPlugin::UpdatePlugins(const util::Time& dt)
	{
		for (auto iter = s_pluginList.begin(); iter != s_pluginList.end(); ++iter)
		{
			if (!(*iter)->Update(dt)) //if an update returns false - that mean something went wrong (or for input: the game is closing) and we should stop everything
				return false;
		}
		return true;
	}

	STATIC void IPlugin::DestroyPlugins()
	{
		s_pluginList.reverse(); //we want to remove them in reverse order
		for (auto iter = s_pluginList.begin(); iter != s_pluginList.end(); ++iter)
		{
			(*iter)->Exit();
			delete(*iter);
		}
	}

	//------------------------------------------------------------------------------

	IPlugin::IPlugin()
	{
	}

	VIRTUAL void IPlugin::Init()
	{
	}
	
	VIRTUAL void IPlugin::Exit()
	{
	}
	
	VIRTUAL bool IPlugin::Update(const util::Time& dt)
	{
		return true;
	}
}