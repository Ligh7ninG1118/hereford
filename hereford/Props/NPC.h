#pragma once
#include "Core/Actor.h"
#include "Util/Reflection.h"
#include <memory>


class NPC : public Actor
{
	REFLECT(NPC)

public:
	NPC(class GameContext* gameCtx);
	~NPC();

private:
	std::unique_ptr<class AnimatedRenderComponent> mPtrAnimRenderComp;
	std::unique_ptr<class AnimationStateMachine> mPtrAnimStateMachine;
};

