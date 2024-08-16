#include "Quaternion.h"
#include "Math.h"

const Quaternion Quaternion::Identity(0.0f, 0.0f, 0.0f, 1.0f);

Quaternion::Quaternion()
	: mX(0.0f), mY(0.0f), mZ(0.0f), mW(1.0f)
{
}

Quaternion::Quaternion(float x, float y, float z, float w)
	: mX(x), mY(y), mZ(z), mW(w)
{
}

Quaternion::Quaternion(float x)
	: mX(x), mY(x), mZ(x), mW(x)
{
}

Quaternion::Quaternion(const Quaternion& rhs)
	: mX(rhs.mX), mY(rhs.mY), mZ(rhs.mZ), mW(rhs.mW)
{
}

Quaternion Quaternion::normalized() const
{
	float magnitude = sqrt(mX * mX + mY * mY + mZ * mZ + mW * mW);

	if (magnitude <= EPSILON)
		return *this;

	return Quaternion(mX / magnitude, mY / magnitude, mZ / magnitude, mW / magnitude);
}

void Quaternion::Normalize()
{
	*this = normalized();
}

Quaternion& Quaternion::operator=(const Quaternion& rhs)
{
	if (this == &rhs)
		return *this;

	mX = rhs.mX;
	mY = rhs.mY;
	mZ = rhs.mZ;
	mW = rhs.mW;

	return *this;
}

bool Quaternion::operator==(const Quaternion& rhs) const
{
	return abs(mX - rhs.mX) < EPSILON 
		&& abs(mY - rhs.mY) < EPSILON 
		&& abs(mZ - rhs.mZ) < EPSILON
		&& abs(mW - rhs.mW) < EPSILON;
}

bool Quaternion::operator!=(const Quaternion& rhs) const
{
	return !(*this==rhs);
}

float Quaternion::operator[](const int& index) const
{
	switch (index)
	{
	case 0:
		return mX;
	case 1:
		return mY;
	case 2:
		return mZ;
	case 3:
		return mW;
	default:
		return -1;
	}
}
