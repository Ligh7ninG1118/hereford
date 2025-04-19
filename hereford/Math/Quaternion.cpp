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

	if (magnitude <= EPISILON)
		return Quaternion::Identity;

	float invMag = 1.0f / magnitude;

	return (*this) * invMag;
}

void Quaternion::Normalize()
{
	*this = normalized();
}

float Quaternion::Dot(const Quaternion& rhs) const
{
	return mX * rhs.mX + mY * rhs.mY + mZ * rhs.mZ + mW * rhs.mW;
}

Quaternion Quaternion::EulerToQuat(const Vector3& euler)
{
	Vector3 radEuler = euler * DEG2RAD;

	float cosRoll = cos(radEuler.mX * 0.5f);
	float sinRoll = sin(radEuler.mX * 0.5f);
	float cosYaw = cos(radEuler.mY * 0.5f);
	float sinYaw = sin(radEuler.mY * 0.5f);
	float cosPitch = cos(radEuler.mZ * 0.5f);
	float sinPitch = sin(radEuler.mZ * 0.5f);

	Quaternion q;
	q.mW = cosRoll * cosPitch * cosYaw + sinRoll * sinPitch * sinYaw;
	q.mX = sinRoll * cosPitch * cosYaw - cosRoll * sinPitch * sinYaw;
	q.mY = cosRoll * sinPitch * cosYaw + sinRoll * cosPitch * sinYaw;
	q.mZ = cosRoll * cosPitch * sinYaw - sinRoll * sinPitch * cosYaw;

	return q;
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
	return abs(mX - rhs.mX) < EPISILON 
		&& abs(mY - rhs.mY) < EPISILON 
		&& abs(mZ - rhs.mZ) < EPISILON
		&& abs(mW - rhs.mW) < EPISILON;
}

bool Quaternion::operator!=(const Quaternion& rhs) const
{
	return !(*this==rhs);
}

Quaternion Quaternion::operator+(const Quaternion& rhs) const
{
	return Quaternion(mX + rhs.mX, mY + rhs.mY, mZ + rhs.mZ, mW + rhs.mW);
}

Quaternion Quaternion::operator-(const Quaternion& rhs) const
{
	return Quaternion(mX - rhs.mX, mY - rhs.mY, mZ - rhs.mZ, mW - rhs.mW);
}

Quaternion Quaternion::operator*(const float& scalar) const
{
	return Quaternion(mX * scalar, mY * scalar, mZ * scalar, mW * scalar);
}

Quaternion Quaternion::operator/(const float& scalar) const
{
	return Quaternion(mX / scalar, mY / scalar, mZ / scalar, mW / scalar);
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

Quaternion operator*(const float& scalar, const Quaternion& quat)
{
	return quat * scalar;
}
