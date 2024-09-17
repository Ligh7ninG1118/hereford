#include <stdio.h>
#include <string>
#include "Core/GameContext.h"

int main(int argc, char* args[])
{
	GameContext* gameCtx = new GameContext();

	if (!gameCtx->Initialize())
	{
		printf("Hereford::Main(): Failed to initialize GameContext\n");
		return -1;
	}

	gameCtx->RunLoop();

	delete gameCtx;
	return 0;
}
