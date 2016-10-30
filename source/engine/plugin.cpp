#include "plugin.h"
#include <assert.h>
#include "utility/helper/macro.h"

namespace baka
{
	STATIC std::list<IPlugin*> IPlugin::s_pluginList;

	STATIC int IPlugin::s_nextPluginTypeId = 0;

	STATIC void IPlugin::AddPlugin(IPlugin* const plugin)
	{
		assert(!FindPlugin(plugin->GetType()));
		plugin->Init();
		s_pluginList.push_back(plugin);

		s_pluginList.sort([](IPlugin* lhs, IPlugin* rhs)
		{
			return lhs->GetPriority() < rhs->GetPriority(); //lowest value goes first
		});
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

	STATIC bool IPlugin::UpdatePlugins(const sf::Time& dt)
	{
		for (auto iter = s_pluginList.begin(); iter != s_pluginList.end(); ++iter)
		{
			if (!(*iter)->Update(dt)) //if an update returns false - that mean something went wrong (or for input: the game is closing) and we should stop everything
				return false;
		}
		return true;
	}

	STATIC const std::list<std::string> IPlugin::DebugDrawPlugins(sf::RenderWindow* pRenWin)
	{
		std::list<std::string> ret;
		for (auto iter = s_pluginList.begin(); iter != s_pluginList.end(); ++iter)
		{
			if (!(*iter)->GetDebugDraw())
				continue;

			std::string temp = (*iter)->DebugRender(pRenWin);
			if (!temp.empty())
			{
				ret.push_back(temp);
			}
		}
		return ret;
	}

	STATIC void IPlugin::DestroyPlugins()
	{
		__todo() //maybe a problem with ordering here due to resorting the list
		s_pluginList.reverse(); //we want to remove them in reverse order
		while (!s_pluginList.empty())
		{
			IPlugin* plug = (*s_pluginList.begin());
			plug->Exit();
			delete plug;
			s_pluginList.erase(s_pluginList.begin());
		}
	}

	//------------------------------------------------------------------------------

	IPlugin::IPlugin()
		: m_bDebugDraw(false)
	{
	}

	VIRTUAL void IPlugin::Init()
	{
	}
	
	VIRTUAL void IPlugin::Exit()
	{
	}
	
	VIRTUAL bool IPlugin::Update(const sf::Time& dt)
	{
		return true;
	}
}