#include "registration.h"
#include "logo_ent.h"
#include "player_ent.h"
#include "tether_ent.h"


namespace app
{
	namespace entity
	{
		//if a script hides behind a namespace we have to set using here so that the register macro can take just the straight class name
		//__todo() //maybe eventually we want to organize things like bvb did with namespaces as part of the part name ie: engine.render.crectpart or engine.ui.button
		//	using namespace renderable;
		//using namespace ui;
		//using namespace box;

		void RegisterAppEntities(baka::entity::EntityPlugin* pEntPlug)
		{
			REGISTER_ENTITY(LogoEnt);
		//	REGISTER_ENTITY(PlayerEnt);
		//	REGISTER_ENTITY(TetherEnt);

			/*REGISTER_PART(CAabbPart);
			REGISTER_PART(CCirclePart);
			REGISTER_PART(CPointsPart);
			REGISTER_PART(CImgPart);*/
		}
	}
}