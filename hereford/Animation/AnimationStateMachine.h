#pragma once
#include <unordered_map>

#include "Util/GameEvent.h"

struct AnimState
{
	unsigned int mAnimIndex;
	bool mShouldLoop;
};


class AnimationStateMachine
{
public:
	AnimationStateMachine(class Animator* inAnimator);
	virtual ~AnimationStateMachine() = default;

	void AddTransitionRule(unsigned int fromAnimIndex, const AnimState& toState);

	void AnimFinishedListener(EventOnAnimFinished inEvent);

private:
	class Animator* mAnimator;

	GameEvent::Subscription<EventOnAnimFinished>* mPtrAnimEvent;

	std::unordered_map<unsigned int, AnimState> mTransitionMap;
};

