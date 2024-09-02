#include "AnimationStateMachine.h"
#include "Animator.h"

AnimationStateMachine::AnimationStateMachine(Animator* inAnimator)
	:
	mAnimator(inAnimator)
{
	// Temp reference for pistol animation
	// 0: Draw, 1: Hide, 2: Static, 3: Reload, 4: Fire

	mTransitionMap[0] = AnimState(2, false);
	mTransitionMap[3] = AnimState(2, false);
	mTransitionMap[4] = AnimState(2, false);
	//TODO: problem! this is essentially broadcast. How do i make it more "direct"?
	mPtrAnimEvent = GameEvent::Subscribe<EventAnimFinished>(std::bind(&AnimationStateMachine::AnimFinishedListener, this, std::placeholders::_1));
}

void AnimationStateMachine::AddTransitionRule(unsigned int fromAnimIndex, const AnimState& toState)
{
	mTransitionMap[fromAnimIndex] = toState;
}

void AnimationStateMachine::AnimFinishedListener(EventAnimFinished inEvent)
{
	if (mTransitionMap.find(inEvent.mAnimIndex) != mTransitionMap.end())
	{
		auto nextAnimState = mTransitionMap[inEvent.mAnimIndex];
		mAnimator->PlayAnimation(nextAnimState.mAnimIndex, nextAnimState.mShouldLoop);
	}
}
