#pragma once


struct Vector2
{
	float mX;
	float mY;

	Vector2();
	explicit Vector2(float x, float y);
	explicit Vector2(int x, int y);
	explicit Vector2(double x, double y);
	Vector2(const Vector2& rhs);

	float Magnitude() const;
	float SqrMagnitude() const;
	Vector2 normalized() const;
	void Normalize();

	float Dot(const Vector2& rhs) const;

	static float Distance(const Vector2& lhs, const Vector2& rhs);
	static float AngleAsDeg(const Vector2& lhs, const Vector2& rhs);
	static float AngleAsRad(const Vector2& lhs, const Vector2& rhs);

	Vector2& operator=(const Vector2& rhs);
	bool operator==(const Vector2* rhs) const;
	Vector2 operator+(const Vector2& rhs) const;
	Vector2 operator-(const Vector2& rhs) const;
	Vector2 operator*(const float& scalar) const;
	Vector2 operator/(const float& scalar) const;

	Vector2& operator+=(const Vector2& rhs);
	Vector2& operator-=(const Vector2& rhs);
	Vector2& operator*=(const float& scalar);
	Vector2& operator/=(const float& scalar);

	// TODO: Add compare operator overloading? (for clamping)
};

