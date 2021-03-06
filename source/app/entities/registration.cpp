#include "registration.h"
#include "logo_ent.h"
#include "player_ent.h"
#include "tether_ent.h"
#include "spawner_ent.h"
#include "liquid_ent.h"
#include "goal_ent.h"
#include "ball_ent.h"
#include "kill_zone_ent.h"
#include "rain_ent.h"
#include "block_ent.h"

namespace app
{
	namespace entity
	{
		//if a script hides behind a namespace we have to set using here so that the register macro can take just the straight class name
		//using namespace ui;
		//using namespace box;

		void RegisterAppEntities(baka::entity::EntityPlugin* pEntPlug)
		{
			REGISTER_ENTITY(LogoEnt);
			REGISTER_ENTITY(PlayerEnt);
			REGISTER_ENTITY(TetherEnt);
			REGISTER_ENTITY(SpawnerEnt);
			REGISTER_ENTITY(LiquidEnt);
			REGISTER_ENTITY(GoalEnt);
			REGISTER_ENTITY(BallEnt);
			REGISTER_ENTITY(KillZoneEnt);
			REGISTER_ENTITY(RainEnt);
			REGISTER_ENTITY(BlockEnt);
		}
	}
}