#include <stdio.h>
#include <string>
#include "Core/GameContext.h"
#include "Asset/AssetManager.h"

#include <crtdbg.h>
#include <vector>

int main(int argc, char* args[])
{
	/*GameContext* gameCtx = new GameContext();

	if (!gameCtx->Initialize())
	{
		printf("Failed to initialize!\n");
		return -1;
	}

	gameCtx->RunLoop();
	
	gameCtx->Shutdown();
	delete gameCtx;
	AssetManager::Destroy();*/

	int* a = new int;
	int* b = new int;
	*a = 5;
	*b = 10;

	delete a;


	_CrtDumpMemoryLeaks();
	return 0;
}
