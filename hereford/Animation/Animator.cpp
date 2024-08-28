#include "Animator.h"
#include "Animation.h"
#include "Bone.h"
#include "Asset/Model.h"
#include <assert.h>
#include <memory>

Animator::Animator(std::vector<class Animation> animations)
	: mCurrentTime(0.0f),
	mDeltaTime(0.0f),
	mCurrentIndex(2),
	mAnimationList(animations),
	mShouldLoop(true),
	mHasFinished(false)
{
	mFinalBoneMatrices.reserve(100);
	for (int i = 0; i < 100; i++)
		mFinalBoneMatrices.push_back(Mat4::Identity);
}

void Animator::UpdateAnimation(float dt)
{
	// Early return for finished one-shot animation
	if (!mShouldLoop && mHasFinished)
		return;

	mDeltaTime = dt;

	mCurrentTime += mAnimationList[mCurrentIndex].GetTicksPerSecond() * dt;

	// For one-shot animation, set time to duration and update bone transform one last time
	if (!mShouldLoop && mCurrentTime >= mAnimationList[mCurrentIndex].GetDuration())
	{
		mHasFinished = true;
		mCurrentTime = mAnimationList[mCurrentIndex].GetDuration();
	}
	else
	{
		mCurrentTime = fmod(mCurrentTime, mAnimationList[mCurrentIndex].GetDuration());
	}

	CalculateBoneTransform(&mAnimationList[mCurrentIndex].GetRootNode(), Mat4::Identity);
}

void Animator::PlayAnimation(Uint32 index, bool shouldLoop)
{
	assert(index >= mAnimationList.size());

	mCurrentIndex = index;
	mCurrentTime = 0.0f;
	mHasFinished = false;
	mShouldLoop = shouldLoop;
}

void Animator::CalculateBoneTransform(const AssimpNodeData* node, Mat4 parentTransform)
{
	const std::string& nodeName = node->mName;
	Mat4 nodeTransform = node->mTransform;

	std::shared_ptr<Bone> bone = mAnimationList[mCurrentIndex].FindBone(nodeName);
	if (bone)
	{
		bone->Update(mCurrentTime);
		nodeTransform = bone->GetLocalTransform();
	}

	Mat4 globalTransform = parentTransform * nodeTransform;
	const auto& boneIDMap = mAnimationList[mCurrentIndex].GetBoneIDMap();
	if (boneIDMap.find(nodeName) != boneIDMap.end())
	{
		int index = boneIDMap.at(nodeName).mID;
		Mat4 offset = boneIDMap.at(nodeName).mOffset;
		mFinalBoneMatrices[index] = globalTransform * offset;
	}

	for (int i = 0; i < node->mChildrenCount; i++)
	{
		CalculateBoneTransform(&node->mChildren[i], globalTransform);
	}
}
