#pragma once
#include <vector>
#include "Math/Math.h"


class Animator
{
public:
	Animator(class Animation* animation);
	void UpdateAnimation(float dt);
	void PlayAnimation(class Animation* pAnim);
	void CalculateBoneTransform(const struct AssimpNodeData* node, Mat4 parentTransform);
	std::vector<Mat4> GetFinalBoneMatrices() const { return mFinalBoneMatrices; }

private:
	std::vector<Mat4> mFinalBoneMatrices;
	class Animation* mCurrentAnimation;
	float mCurrentTime;
	float mDeltaTime;
};

