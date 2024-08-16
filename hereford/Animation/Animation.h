#pragma once

#include "Math/Math.h"
#include <string>
#include <vector>
#include <map>

struct AssimpNodeData
{
	Mat4 mTransform;
	std::string mName;
	int mChildrenCount;
	std::vector<AssimpNodeData> mChildren;
};

class Animation
{
public:
	Animation() = default;
	Animation(const std::string& animPath, class Model* model);
	~Animation();

	struct Bone* FindBone(const std::string& boneName);

	inline float GetTicksPerSecond() const { return mTicksPerSec; }
	inline float GetDuration() const { return mDuration; }
	inline const AssimpNodeData& GetRootNode() const { return mRootNode; }
	inline auto& GetBoneIDMap() const { return mBoneInfoMap; }

private:
	float mDuration;
	float mTicksPerSec;
	std::vector<struct Bone> mBones;
	AssimpNodeData mRootNode;
	std::map<std::string, struct BoneInfo> mBoneInfoMap;

	void ReadMissingBones(const struct aiAnimation* animation, Model& model);
	void ReadHierarchyData(AssimpNodeData& dest, const struct aiNode* src);
};

