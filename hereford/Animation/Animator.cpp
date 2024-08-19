#include "Animator.h"
#include "Animation.h"
#include "Bone.h"
#include "Asset/Model.h"
#include <assert.h>
#include <memory>

Animator::Animator(std::vector<class Animation> animations)
	: mCurrentTime(0.0f),
	mDeltaTime(0.0f),
	mAnimationList(animations)
{
	mFinalBoneMatrices.reserve(100);
	for (int i = 0; i < 100; i++)
		mFinalBoneMatrices.push_back(Mat4::Identity);
}

void Animator::UpdateAnimation(float dt)
{
	mDeltaTime = dt;
	
	mCurrentTime += mAnimationList[mCurrentIndex].GetTicksPerSecond() * dt;
	mCurrentTime = fmod(mCurrentTime, mAnimationList[mCurrentIndex].GetDuration());
	CalculateBoneTransform(&mAnimationList[mCurrentIndex].GetRootNode(), Mat4::Identity);
}

void Animator::PlayAnimation(Uint32 index)
{
	assert(index >= mAnimationList.size());

	mCurrentIndex = index;
	mCurrentTime = 0.0f;
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
