#include "knock_off_state.h"

#include "input/input_plugin.h"
#include "render/render_plugin.h"
#include "state/state_plugin.h"
#include "entity/entity_plugin.h"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "rapidjson/FileReadStream.h"
#include <cstdio>
#include "resource/json.h"

#include "input/input_event.h"
#include "engine/b2djson/b2dJsonImage.h"
#include "render/box2d-sfml.h"
#include "entities/goal_ent.h"

namespace app
{
	namespace state
	{
		KnockOffState::KnockOffState()
		{
			m_vPlayer[0] = m_vPlayer[1] = null;
			this->m_name = "KnockOff";
		}

		VIRTUAL void KnockOffState::Init()
		{
			m_pPhysicsPlugin = FIND_PLUGIN(baka::physics::PhysicsPlugin)

			sf::View& view = m_pPhysicsPlugin->GetView();
			float scale = 30.0f;
			view.setSize(1280/scale, 720/scale);
			view.setCenter(0, -32);
			//view.zoom(1.25);

			std::string path = "assets/worlds/";
			std::string file = "testbed.json";

			b2dJson* json = m_pPhysicsPlugin->LoadWorld(path, file);

			baka::entity::EntityPlugin* pEntPlug = FIND_PLUGIN(baka::entity::EntityPlugin)
			//b2Body* pPlayerBody = json->getBodyByName("player");
		//	m_pPlayer = CreateEntityMacro(pEntPlug, entity::PlayerEnt);
			//m_pPlayer->Setup(pPlayerBody);
		}

		VIRTUAL void KnockOffState::Exit()
		{
			baka::entity::EntityPlugin* pEntPlug = static_cast<baka::entity::EntityPlugin*>(baka::IPlugin::FindPlugin(baka::entity::EntityPlugin::Type));
			if (pEntPlug)
				pEntPlug->DestroyAllEntities();
		}

		VIRTUAL const std::string KnockOffState::DebugRender(sf::RenderWindow* pRenWin)
		{
			std::string ret;
			return ret;
		}

		VIRTUAL void KnockOffState::Update(const sf::Time& dt)
		{
			
		}

	}
}