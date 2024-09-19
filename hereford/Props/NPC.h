#pragma once
#include "Core/Actor.h"
#include "Util/Reflection.h"


class NPC : public Actor
{
	REFLECT(NPC)

public:
	NPC(class GameContext* gameCtx);
	~NPC();

private:
	class AnimatedRenderComponent* mPtrAnimRenderComp;
	class AnimationStateMachine* mPtrAnimStateMachine;
};

