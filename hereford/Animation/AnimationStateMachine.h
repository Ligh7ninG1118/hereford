#pragma once
#include "Core/Component.h"
#include "Util/GameEvent.h"
#include <unordered_map>
#include <memory>

struct AnimState
{
	uint32 mAnimIndex;
	bool mShouldLoop;
};


class AnimationStateMachine : public Component
{
public:
	AnimationStateMachine(class Actor* owner, std::unique_ptr<class Animator> inAnimator);
	virtual ~AnimationStateMachine() = default;

	void Update(float deltaTime) override;

	void AddTransitionRule(unsigned int fromAnimIndex, const AnimState& toState);

	void PlayAnimation(uint32 index, bool shouldLoop = true, float duration = -1.0f);

	std::vector<Mat4> GetFinalBoneMatrices() const;


private:
	void AnimFinishedListener(EventOnAnimFinished inEvent);

	std::unique_ptr<class Animator> mPtrAnimator;

	GameEvent::Subscription<EventOnAnimFinished>* mPtrAnimEvent;

	std::unordered_map<unsigned int, AnimState> mTransitionMap;
};

