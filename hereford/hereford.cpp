#include <cstdio>
#include <string>
#include <memory>
#include "Core/GameContext.h"

int main(int argc, char* args[])
{
	auto gameCtx = std::make_unique<GameContext>();

	if (!gameCtx->Initialize())
	{
		printf("Hereford::Main(): Failed to initialize GameContext\n");
		return -1;
	}

	gameCtx->RunLoop();

	return 0;
}
