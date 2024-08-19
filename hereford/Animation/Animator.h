#pragma once
#include <vector>
#include "Math/Math.h"


class Animator
{
public:
	Animator(std::vector<class Animation> animations);
	void UpdateAnimation(float dt);
	void PlayAnimation(Uint32 index);
	void CalculateBoneTransform(const struct AssimpNodeData* node, Mat4 parentTransform);
	std::vector<Mat4> GetFinalBoneMatrices() const { return mFinalBoneMatrices; }

private:
	std::vector<Mat4> mFinalBoneMatrices;
	std::vector<class Animation> mAnimationList;
	int mCurrentIndex;
	float mCurrentTime;
	float mDeltaTime;
};

