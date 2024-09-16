#include <stdio.h>
#include <string>
#include "Core/GameContext.h"
#include "Asset/AssetManager.h"

int main(int argc, char* args[])
{

	GameContext* gameCtx = new GameContext();

	if (!gameCtx->Initialize())
	{
		printf("Failed to initialize!\n");
		return -1;
	}

	gameCtx->RunLoop();
	
	gameCtx->Shutdown();
	delete gameCtx;
	return 0;
}
