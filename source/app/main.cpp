#include "engine/engine.h"
#include "engine/state/setup_state.h"
#include "utility/types/types.h"
#include <vld.h>


int main()
{
	baka::state::SetupState* pSetupState = new baka::state::SetupState();

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
