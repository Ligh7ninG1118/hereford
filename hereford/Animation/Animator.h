#pragma once
#include <vector>
#include "Math/Math.h"


class Animator
{
public:
	Animator(std::vector<class Animation> animations);
	void UpdateAnimation(float dt);
	void PlayAnimation(uint32 index, bool shouldLoop = true, float duration = -1.0f);
	void CalculateBoneTransform(const struct AssimpNodeData* node, Mat4 parentTransform);
	std::vector<Mat4> GetFinalBoneMatrices() const { return mFinalBoneMatrices; }

	inline void SetShouldLoop(bool in) { mShouldLoop = in; }

private:
	std::vector<Mat4> mFinalBoneMatrices;
	std::vector<class Animation> mAnimationList;
	int mCurrentIndex;
	float mCurrentTime;
	float mPlaybackSpeed;

	//TODO: place this inside Animation class?
	bool mShouldLoop;
	bool mHasFinished;
};

