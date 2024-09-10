#pragma once
#include "Core/Actor.h"
class NPC : public Actor
{
public:
	NPC(class GameContext* gameCtx);
	~NPC();

private:
	class AnimatedRenderComponent* mPtrAnimRenderComp;
	class AnimationStateMachine* mPtrAnimStateMachine;
};

