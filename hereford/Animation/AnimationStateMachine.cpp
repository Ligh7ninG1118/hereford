#include "AnimationStateMachine.h"
#include "Animator.h"

AnimationStateMachine::AnimationStateMachine(Animator* inAnimator)
	:
	mAnimator(inAnimator)
{
	mTransitionMap[0] = AnimState(1, false);
	mTransitionMap[1] = AnimState(3, false);
	mTransitionMap[3] = AnimState(4, true);
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
