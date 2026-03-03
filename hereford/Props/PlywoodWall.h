#pragma once
#include "Core/Actor.h"
#include "Util/Reflection.h"
#include <memory>

class PlywoodWall : public Actor
{
	REFLECT(PlywoodWall)

public:
	PlywoodWall(class GameContext* gameCtx, int type = 1);
	~PlywoodWall();

private:
	int mType;
	std::unique_ptr<class RenderComponent> mPtrRenderComp;

};
