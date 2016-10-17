#pragma once
#include "utility/time/util_time.h"
#include <list>
#include <assert.h>
#include "helper/macro.h"
#include "sfml/system/clock.hpp"
#include <SFML/Graphics.hpp>

namespace baka
{
	__todo() //probably need to move this somewhere else?
	enum EPluginOrder
	{
		EPO_INVALID = -1,
		EPO_INPUT,
		EPO_PHYSICS,
		EPO_COMPONENT,
		EPO_STATE,
		EPO_RENDER = 9999
	};

	class IPlugin
	{
	public:
		//Internal
	private:
		static std::list<IPlugin*> s_pluginList;

	protected:
		static int s_nextPluginTypeId;
		bool m_bDebugDraw;

		//External
	public:
		static void AddPlugin(IPlugin* const plugin);
		static IPlugin* FindPlugin(const int& type);
		static bool UpdatePlugins(const sf::Time& dt);
		static const std::list<std::string> DebugDrawPlugins(sf::RenderWindow* pRenWin);
		static void DestroyPlugins();

		static const int Type = 0;
		virtual int GetType() const { return Type; }
		virtual const EPluginOrder GetPriority() const { return EPO_INVALID; }
		const bool GetDebugDraw() const { return m_bDebugDraw; }
		void SetDebugDraw(const bool b) { m_bDebugDraw = b; }

		//------------------------------------------------------------------------------

	public:
		IPlugin();
		virtual ~IPlugin() {};

		virtual void Init();
		virtual void Exit();
		virtual void Reload() { Exit(); Init(); }
		virtual bool Update(const sf::Time& dt);
		virtual const std::string DebugRender(sf::RenderWindow* pRenWin) { return ""; }; //the string returned is debug text to show

#define DECLARE_PLUGIN_TYPE_INFO(CLASS)									\
			typedef CLASS class_t;										\
			static const int Type;										\
			virtual inline int GetType() const { return Type; }			
#define DEFINE_PLUGIN_TYPE_INFO(CLASS)									\
			const int CLASS::Type = ++IPlugin::s_nextPluginTypeId;			
	};
}