#include "Math.h"
#include "Vector3.h"

const Vector3 Vector3::Zero(0.0f, 0.0f, 0.0f);
const Vector3 Vector3::One(1.0f, 1.0f, 1.0f);
const Vector3 Vector3::Up(0.0f, 1.0f, 0.0f);
const Vector3 Vector3::Down(0.0f, -1.0f, 0.0f);
const Vector3 Vector3::Left(-1.0f, 0.0f, 0.0f);
const Vector3 Vector3::Right(1.0f, 0.0f, 0.0f);
const Vector3 Vector3::Forward(0.0f, 0.0f, -1.0f);
const Vector3 Vector3::Back(0.0f, 0.0f, 1.0f);


Vector3::Vector3()
	: mX(0.0f), mY(0.0f), mZ(0.0f)
{
}

Vector3::Vector3(float x, float y, float z)
	: mX(x), mY(y), mZ(z)
{
}

Vector3::Vector3(int x, int y, int z)
	: mX(static_cast<float>(x)), mY(static_cast<float>(y)), mZ(static_cast<float>(z))
{
}

Vector3::Vector3(double x, double y, double z)
	: mX(static_cast<float>(x)), mY(static_cast<float>(y)), mZ(static_cast<float>(z))
{
}

Vector3::Vector3(float x)
	: mX(x), mY(x), mZ(x)
{
}

Vector3::Vector3(int x)
	: mX(static_cast<float>(x)), mY(static_cast<float>(x)), mZ(static_cast<float>(x))
{
}

Vector3::Vector3(double x)
	: mX(static_cast<float>(x)), mY(static_cast<float>(x)), mZ(static_cast<float>(x))
{
}

Vector3::Vector3(const Vector3& rhs)
	: mX(rhs.mX), mY(rhs.mY), mZ(rhs.mZ)
{
}

float Vector3::Magnitude() const
{
	return sqrt(SqrMagnitude());
}

float Vector3::SqrMagnitude() const
{
	return mX * mX + mY * mY + mZ * mZ;
}

Vector3 Vector3::normalized() const
{
	float magnitude = this->Magnitude();
	if (magnitude <= EPSILON)
		return *this;
	return Vector3(mX / magnitude, mY / magnitude, mZ / magnitude);
}

void Vector3::Normalize()
{
	*this = normalized();
}

float Vector3::Dot(const Vector3& rhs) const
{
	return this->mX * rhs.mX + this->mY * rhs.mY + this->mZ *rhs.mZ;
}

Vector3 Vector3::Cross(const Vector3& rhs) const
{
	return Vector3(this->mY * rhs.mZ - this->mZ * rhs.mY,
					this->mZ * rhs.mX - this->mX * rhs.mZ,
					this->mX * rhs.mY - this->mY * rhs.mX);
}

float Vector3::Distance(const Vector3& lhs, const Vector3& rhs)
{
	return (lhs - rhs).Magnitude();
}

float Vector3::AngleAsDeg(const Vector3& lhs, const Vector3& rhs)
{
	return AngleAsRad(lhs, rhs) * RAD2DEG;
}

float Vector3::AngleAsRad(const Vector3& lhs, const Vector3& rhs)
{
	return acosf(lhs.Dot(rhs) / (lhs.Magnitude() * rhs.Magnitude()));
}

Vector3& Vector3::operator=(const Vector3& rhs)
{
	if (this == &rhs)
		return *this;

	mX = rhs.mX;
	mY = rhs.mY;
	mZ = rhs.mZ;

	return *this;
}

bool Vector3::operator==(const Vector3& rhs) const
{
	return abs(mX - rhs.mX) < EPSILON && abs(mY - rhs.mY) < EPSILON && abs(mZ - rhs.mZ) < EPSILON;
}

bool Vector3::operator!=(const Vector3& rhs) const
{
	return !(*this==rhs);
}

Vector3 Vector3::operator+(const Vector3& rhs) const
{
	return Vector3(mX + rhs.mX, mY + rhs.mY, mZ + rhs.mZ);
}

Vector3 Vector3::operator-(const Vector3& rhs) const
{
	return Vector3(mX - rhs.mX, mY - rhs.mY, mZ - rhs.mZ);
}

Vector3 Vector3::operator-() const
{
	return Vector3(-mX, -mY, -mZ);
}

Vector3 Vector3::operator*(const float& scalar) const
{
	return Vector3(mX * scalar, mY * scalar, mZ * scalar);
}

Vector3 Vector3::operator/(const float& scalar) const
{
	return Vector3(mX / scalar, mY / scalar, mZ / scalar);
}

float Vector3::operator[](const int& index) const
{
	switch (index)
	{
	case 0:
		return mX;
	case 1:
		return mY;
	case 2:
		return mZ;
	default:
		return -1;
	}
}

Vector3& Vector3::operator+=(const Vector3& rhs)
{
	mX += rhs.mX;
	mY += rhs.mY;
	mZ += rhs.mZ;
	return *this;
}

Vector3& Vector3::operator-=(const Vector3& rhs)
{
	mX -= rhs.mX;
	mY -= rhs.mY;
	mZ -= rhs.mZ;
	return *this;
}

Vector3& Vector3::operator*=(const float& scalar)
{
	mX *= scalar;
	mY *= scalar;
	mZ *= scalar;
	return *this;
}

Vector3& Vector3::operator/=(const float& scalar)
{
	mX /= scalar;
	mY /= scalar;
	mZ /= scalar;
	return *this;
}

const Vector3& operator*(const float& scalar, const Vector3& vec3)
{
	return vec3 * scalar;
}
