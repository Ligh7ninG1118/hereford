#include "Animation.h"
#include "Asset/Model.h"
#include "Bone.h"
#include "Math/MathAssimpHelper.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <assert.h>


Animation::Animation(const std::string& animPath, Model* model)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(animPath, aiProcess_Triangulate);
	assert(scene && scene->mRootNode);
	auto animation = scene->mAnimations[0];
	mDuration = animation->mDuration;
	mTicksPerSec = animation->mTicksPerSecond;
	ReadHierarchyData(mRootNode, scene->mRootNode);
	ReadMissingBones(animation, *model);
}

Animation::~Animation()
{
}

Bone* Animation::FindBone(const std::string& boneName)
{
	auto it = std::find_if(mBones.begin(), mBones.end(),
		[&](const Bone& bone)
		{
			return bone.GetBoneName() == boneName;
		});
	if (it == mBones.end())
		return nullptr;
	else
		return &(*it);
}

void Animation::ReadMissingBones(const aiAnimation* animation, Model& model)
{
	int size = animation->mNumChannels;
	auto& boneInfoMap = model.GetBoneInfoMap();
	int& boneCount = model.GetBoneCount();

	for (int i = 0; i < size; i++)
	{
		auto channel = animation->mChannels[i];
		std::string boneName = channel->mNodeName.data;
		if (boneInfoMap.find(boneName) == boneInfoMap.end())
		{
			boneInfoMap[boneName].id = boneCount;
			boneCount++;
		}
		mBones.push_back(Bone(channel->mNodeName.data, boneInfoMap[channel->mNodeName.data].id, channel));

	}
	mBoneInfoMap = boneInfoMap;
}

void Animation::ReadHierarchyData(AssimpNodeData& dest, const aiNode* src)
{
	assert(src);

	dest.mName = src->mName.data;
	dest.mTransform = MathAssimpHelper::ConvertAssimpMat4(src->mTransformation);
	dest.mChildrenCount = src->mNumChildren;

	for (int i = 0; i < src->mNumChildren; i++)
	{
		AssimpNodeData newData;
		ReadHierarchyData(newData, src->mChildren[i]);
		dest.mChildren.push_back(newData);
	}
}
