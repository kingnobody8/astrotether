#include "setup_state.h"
#include "engine/state/state_plugin.h"


#include "utility/resource/json.h"
#include "asset/loader.h"
#include "asset/resource_path.h"
#include "render/render_plugin.h"
#include "render/camera.h"
#include "render/render_layer.h"

namespace app
{
	namespace state
	{
		SetupState::SetupState()
		{
			this->m_name = "SetupState";
		}

		VIRTUAL SetupState::~SetupState()
		{
		}

		VIRTUAL void SetupState::Init()
		{
			//engine::render::RenderPlugin* plug = static_cast<engine::render::RenderPlugin*>(engine::IPlugin::FindPlugin(engine::render::RenderPlugin::Type));
			//plug->TransitionState(new SetupState());

			/*util::JSON rconfig = engine::asset::FileLoaderJson(getResourcePath() + "assets/config/render_config.json");
			for (int i = 0; i < (int)rconfig["camera_list"].Size(); ++i)
			{
				engine::render::CCamera::CreateCamera(rconfig["camera_list"][i].GetString());
			}
			for (int i = 0; i < (int)rconfig["render_layer_list"].Size(); ++i)
			{
				std::string name = rconfig["render_layer_list"][i]["name"].GetString();
				std::string camera = rconfig["render_layer_list"][i]["camera"].GetString();
				engine::render::CRenderLayer::CreateLayer(name, i, engine::render::CCamera::FindCamera(camera));
			}*/
		}

		VIRTUAL void SetupState::Exit()
		{
		}
	}
}