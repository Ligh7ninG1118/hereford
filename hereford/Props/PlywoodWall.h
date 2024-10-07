#pragma once
#include "Core/Actor.h"
#include "Util/Reflection.h"

class PlywoodWall : public Actor
{
	REFLECT(PlywoodWall)

public:
	PlywoodWall(class GameContext* gameCtx, int type = 1);
	~PlywoodWall();

private:
	int mType;
	class RenderComponent* mPtrRenderComp;

};
