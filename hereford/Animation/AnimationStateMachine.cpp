#include "AnimationStateMachine.h"
#include "Core/Actor.h"
#include "Animator.h"

AnimationStateMachine::AnimationStateMachine(Actor* owner, std::unique_ptr<Animator> inAnimator)
	:
	Component(owner),
	mPtrAnimator(std::move(inAnimator))
{
	//TODO: problem! this is essentially broadcast. How do i make it more "direct"?
	mPtrAnimEvent = GameEvent::Subscribe<EventOnAnimFinished>(std::bind(&AnimationStateMachine::AnimFinishedListener, this, std::placeholders::_1));
}

void AnimationStateMachine::Update(float deltaTime)
{
	if (mAnimator != nullptr)
		mAnimator->UpdateAnimation(deltaTime);
}

void AnimationStateMachine::AddTransitionRule(unsigned int fromAnimIndex, const AnimState& toState)
{
	mTransitionMap[fromAnimIndex] = toState;
}

void AnimationStateMachine::PlayAnimation(uint32 index, bool shouldLoop, float duration)
{
	mAnimator->PlayAnimation(index, shouldLoop, duration);
}

void AnimationStateMachine::AnimFinishedListener(EventOnAnimFinished inEvent)
{
	if (mTransitionMap.find(inEvent.mAnimIndex) != mTransitionMap.end())
	{
		auto nextAnimState = mTransitionMap[inEvent.mAnimIndex];
		mAnimator->PlayAnimation(nextAnimState.mAnimIndex, nextAnimState.mShouldLoop);
	}
}
