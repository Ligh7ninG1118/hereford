#include "Animation.h"
#include "Asset/Model.h"
#include "Bone.h"
#include "Math/MathAssimpHelper.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/StringUtils.h>
#include <assert.h>



Animation::Animation(aiAnimation* aiAnim, const aiScene* aiScene, std::weak_ptr<Model> model)
{
	mDuration = aiAnim->mDuration;
	mTicksPerSec = aiAnim->mTicksPerSecond;
	ReadHierarchyData(mRootNode, aiScene->mRootNode);
	ReadMissingBones(aiAnim, *model.lock().get());
}

[[deprecated("Use static func LoadAnimations")]]
Animation::Animation(const std::string& animPath, Model* model)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(animPath, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);
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

std::vector<Animation> Animation::LoadAnimations(const std::string& animPath, std::weak_ptr<Model> model)
{
	std::vector<Animation> animClips;

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(animPath, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);
	assert(scene && scene->mRootNode);

	int num = scene->mNumAnimations;

	for (int i = 0; i < num; i++)
	{
		auto aiAnim = scene->mAnimations[i];
		animClips.push_back(Animation(aiAnim, scene, model));
	}

	return animClips;
}

std::shared_ptr<Bone> Animation::FindBone(const std::string& boneName)
{
	auto it = std::find_if(mBones.begin(), mBones.end(),
		[&](const Bone& bone)
		{
			return bone.GetBoneName() == boneName;
		});
	if (it == mBones.end())
		return nullptr;
	else
		return std::make_shared<Bone>(*it);
}



void Animation::ReadMissingBones(const aiAnimation* animation, Model& model)
{
	int size = animation->mNumChannels;
	std::map<std::string, BoneInfo> boneInfoMap = model.GetBoneInfoMap();
	int& boneCount = model.GetBoneCount();

	for (int i = 0; i < size; i++)
	{
		auto channel = animation->mChannels[i];
		std::string boneName = channel->mNodeName.data;
		if (boneInfoMap.find(boneName) == boneInfoMap.end())
		{
			boneInfoMap[boneName].mID = boneCount;
			boneCount++;
		}

		mBones.push_back(Bone(channel->mNodeName.data, boneInfoMap[channel->mNodeName.data].mID, channel));

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
