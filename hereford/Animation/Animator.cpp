#include "Animator.h"
#include "Animation.h"
#include "Bone.h"
#include "Asset/Model.h"
#include <memory>

Animator::Animator(Animation* animation)
	: mCurrentTime(0.0f),
	mCurrentAnimation(animation)
{
	mFinalBoneMatrices.reserve(100);
	for (int i = 0; i < 100; i++)
		mFinalBoneMatrices.push_back(Mat4::Identity);
}

void Animator::UpdateAnimation(float dt)
{
	mDeltaTime = dt;
	if (mCurrentAnimation)
	{
		mCurrentTime += mCurrentAnimation->GetTicksPerSecond() * dt;
		mCurrentTime = fmod(mCurrentTime, mCurrentAnimation->GetDuration());
		CalculateBoneTransform(&mCurrentAnimation->GetRootNode(), Mat4::Identity);
	}
}

void Animator::PlayAnimation(Animation* pAnim)
{
	mCurrentAnimation = pAnim;
	mCurrentTime = 0.0f;
}

void Animator::CalculateBoneTransform(const AssimpNodeData* node, Mat4 parentTransform)
{
	const std::string& nodeName = node->mName;
	Mat4 nodeTransform = node->mTransform;

	std::shared_ptr<Bone> bone = mCurrentAnimation->FindBone(nodeName);
	if (bone)
	{
		bone->Update(mCurrentTime);
		nodeTransform = bone->GetLocalTransform();
	}

	Mat4 globalTransform = parentTransform * nodeTransform;
	const auto& boneIDMap = mCurrentAnimation->GetBoneIDMap();
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