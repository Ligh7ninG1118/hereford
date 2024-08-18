#include "Bone.h"
#include "Math/MathAssimpHelper.h"
#include <assert.h>
#include <assimp/anim.h>

Bone::Bone(const std::string& name, int id, const aiNodeAnim* channel)
	: mName(name),
	mID(id),
	mLocalTransform(Mat4::Identity)
{
	mNumPosition = channel->mNumPositionKeys;
	for (int posIndex = 0; posIndex < mNumPosition; posIndex++)
	{
		KeyPosition data;
		data.mPosition = MathAssimpHelper::ConvertAssimpVec3(channel->mPositionKeys[posIndex].mValue);
		data.mTimeStamp = channel->mPositionKeys[posIndex].mTime;

		mPositions.push_back(data);
	}

	mNumRotation = channel->mNumRotationKeys;
	for (int rotIndex = 0; rotIndex < mNumRotation; rotIndex++)
	{
		KeyRotation data;
		data.mRotation = MathAssimpHelper::ConvertAssimpQuat(channel->mRotationKeys[rotIndex].mValue);
		data.mTimeStamp = channel->mRotationKeys[rotIndex].mTime;

		mRotations.push_back(data);
	}

	mNumScale = channel->mNumScalingKeys;
	for (int scaleIndex = 0; scaleIndex < mNumScale; scaleIndex++)
	{
		KeyScale data;
		data.mScale = MathAssimpHelper::ConvertAssimpVec3(channel->mScalingKeys[scaleIndex].mValue);
		data.mTimeStamp = channel->mScalingKeys[scaleIndex].mTime;

		mScales.push_back(data);
	}
}

void Bone::Update(float animTime)
{
	Mat4 translation = InterpolatePosition(animTime);
	Mat4 rotation = InterpolateRotation(animTime);
	Mat4 scaling = InterpolateScaling(animTime);
	mLocalTransform = translation * rotation * scaling;
}

int Bone::GetPositionIndex(float animTime)
{
	for (int i = 0; i < mNumPosition - 1; i++)
	{
		if (animTime < mPositions[i + 1].mTimeStamp)
			return i;
	}
	assert(0);
}

int Bone::GetRotationIndex(float animTime)
{
	for (int i = 0; i < mNumRotation - 1; i++)
	{
		if (animTime < mRotations[i + 1].mTimeStamp)
			return i;
	}
	assert(0);
}

int Bone::GetScaleIndex(float animTime)
{
	for (int i = 0; i < mNumScale - 1; i++)
	{
		if (animTime < mScales[i + 1].mTimeStamp)
			return i;
	}
	assert(0);
}

float Bone::GetLerpFactor(float lastTimeStamp, float nextTimeStamp, float animTime)
{
	float midwayLen = animTime - lastTimeStamp;
	float frameDiff = nextTimeStamp - lastTimeStamp;
	return Math::Clamp(midwayLen / frameDiff, 0.0f, 1.0f);
}

Mat4 Bone::InterpolatePosition(float animTime)
{
	if (mNumPosition == 1)
		return Mat4(1.0f).Translate(mPositions[0].mPosition);

	int p0Index = GetPositionIndex(animTime);
	int p1Index = p0Index + 1;

	float lerpFactor = GetLerpFactor(mPositions[p0Index].mTimeStamp, mPositions[p1Index].mTimeStamp, animTime);
	Vec3 finalPos = Math::Lerp<Vec3>(mPositions[p0Index].mPosition, mPositions[p1Index].mPosition, lerpFactor);

	return Mat4(1.0f).Translate(finalPos);
}

Mat4 Bone::InterpolateRotation(float animTime)
{
	if (mNumRotation == 1)
		return Mat4(1.0f).Rotate(mRotations[0].mRotation);

	int p0Index = GetRotationIndex(animTime);
	int p1Index = p0Index + 1;

	float lerpFactor = GetLerpFactor(mRotations[p0Index].mTimeStamp, mRotations[p1Index].mTimeStamp, animTime);
	Quat finalRot = Math::Slerp(mRotations[p0Index].mRotation, mRotations[p1Index].mRotation, lerpFactor);

	return Mat4(1.0f).Rotate(finalRot);
}

Mat4 Bone::InterpolateScaling(float animTime)
{
	if (mNumScale == 1)
		return Mat4(1.0f).Scale(mScales[0].mScale);

	int p0Index = GetScaleIndex(animTime);
	int p1Index = p0Index + 1;

	float lerpFactor = GetLerpFactor(mScales[p0Index].mTimeStamp, mScales[p1Index].mTimeStamp, animTime);
	Vec3 finalScale = Math::Lerp<Vec3>(mScales[p0Index].mScale, mScales[p1Index].mScale, lerpFactor);

	return Mat4(1.0f).Scale(finalScale);
}
