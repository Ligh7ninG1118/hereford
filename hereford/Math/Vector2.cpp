#include "Math.h"
#include "Vector2.h"

Vector2::Vector2()
	: mX(0.0f), mY(0.0f)
{
}

Vector2::Vector2(float x, float y)
	: mX(x), mY(y)
{
}

Vector2::Vector2(int x, int y)
{
}

Vector2::Vector2(double x, double y)
{
}

Vector2::Vector2(const Vector2& rhs)
{
}

float Vector2::Magnitude() const
{
	return sqrt(SqrMagnitude());
}

float Vector2::SqrMagnitude() const
{
	return mX * mX + mY * mY;
}

Vector2 Vector2::normalized() const
{
	float magnitude = this->Magnitude();
	if (magnitude <= EPSILON)
		return *this;
	return Vector2(mX / magnitude, mY / magnitude);
}

void Vector2::Normalize()
{
	*this = normalized();
}

float Vector2::Dot(const Vector2& rhs) const
{
	return this->mX * rhs.mX + this->mY * rhs.mY;
}

float Vector2::Distance(const Vector2& lhs, const Vector2& rhs)
{
	return (lhs - rhs).Magnitude();
}

float Vector2::AngleAsDeg(const Vector2& lhs, const Vector2& rhs)
{
	return AngleAsRad(lhs, rhs) * RAD2DEG;
}

float Vector2::AngleAsRad(const Vector2& lhs, const Vector2& rhs)
{
	return acosf(lhs.Dot(rhs) / (lhs.Magnitude() * rhs.Magnitude()));
}

Vector2& Vector2::operator=(const Vector2& rhs)
{
	if (this == &rhs)
		return *this;

	mX = rhs.mX;
	mY = rhs.mY;

	return *this;
}

bool Vector2::operator==(const Vector2* rhs) const
{
	return (mX == rhs->mX) && (mY == rhs->mY);
}

Vector2 Vector2::operator+(const Vector2& rhs) const
{
	return Vector2(mX + rhs.mX, mY + rhs.mY);
}

Vector2 Vector2::operator-(const Vector2& rhs) const
{
	return Vector2(mX - rhs.mX, mY - rhs.mY);
}

Vector2 Vector2::operator*(const float& scalar) const
{
	return Vector2(mX * scalar, mY * scalar);
}

Vector2 Vector2::operator/(const float& scalar) const
{
	return Vector2(mX / scalar, mY / scalar);
}

Vector2& Vector2::operator+=(const Vector2& rhs)
{
	mX += rhs.mX;
	mY += rhs.mY;
	return *this;
}

Vector2& Vector2::operator-=(const Vector2& rhs)
{
	mX -= rhs.mX;
	mY -= rhs.mY;
	return *this;
}

Vector2& Vector2::operator*=(const float& scalar)
{
	mX *= scalar;
	mY *= scalar;
	return *this;
}

Vector2& Vector2::operator/=(const float& scalar)
{
	mX /= scalar;
	mY /= scalar;
	return *this;
}
