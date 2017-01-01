#include "engine/engine.h"
#include "engine/state/setup_state.h"
#include "utility/types/types.h"
#include "states/testbed_state.h"
#include <vld.h>


int main()
{
	baka::state::SetupState* pSetupState = new baka::state::SetupState();
	pSetupState->SetTransitionState(new app::state::TestbedState());

	baka::Engine* pEngine = baka::Engine::Get();

	pEngine->Init(pSetupState);

	while (!pEngine->GetQuit())
	{
		pEngine->RunFrame(null);
	}

	pEngine->Exit();
	pEngine = null;

	return EXIT_SUCCESS;
}
