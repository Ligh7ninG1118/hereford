#pragma once
#include "Math/Math.h"
#include <vector>
#include <string>

struct KeyPosition
{
	Vec3 mPosition;
	float mTimeStamp;
};

struct KeyRotation
{
	Quat mRotation;
	float mTimeStamp;
};

struct KeyScale
{
	Vec3 mScale;
	float mTimeStamp;
};

class Bone
{
public:
	Bone(const std::string& name, int id, const struct aiNodeAnim* channel);
	void Update(float animTime);

	int GetPositionIndex(float animTime);
	int GetRotationIndex(float animTime);
	int GetScaleIndex(float animTime);

	Mat4 GetLocalTransform() const { return mLocalTransform; }
	std::string GetBoneName() const { return mName; }
	int GetBoneID() const { return mID; }

private:
	std::vector<KeyPosition> mPositions;
	std::vector<KeyRotation> mRotations;
	std::vector<KeyScale> mScales;
	int mNumPosition;
	int mNumRotation;
	int mNumScale;

	Mat4 mLocalTransform;
	std::string mName;
	int mID;

	float GetLerpFactor(float lastTimeStamp, float nextTimeStamp, float animTime);
	Mat4 InterpolatePosition(float animTime);
	Mat4 InterpolateRotation(float animTime);
	Mat4 InterpolateScaling(float animTime);
};

