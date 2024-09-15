#pragma once
struct Vector4
{
	float mX;
	float mY;
	float mZ;
	float mW;

	Vector4();
	Vector4(float x, float y, float z, float w);
	Vector4(float x);
	float Magnitude() const;
	float SqrMagnitude() const;
	Vector4 normalized() const;
	void Normalize();

	float Dot(const Vector4& rhs) const;
	Vector4 Cross(const Vector4& rhs) const;

	static float Distance(const Vector4& lhs, const Vector4& rhs);
	static float AngleAsDeg(const Vector4& lhs, const Vector4& rhs);
	static float AngleAsRad(const Vector4& lhs, const Vector4& rhs);

	Vector4& operator=(const Vector4& rhs);
	bool operator==(const Vector4* rhs) const;
	Vector4 operator+(const Vector4& rhs) const;
	Vector4 operator-(const Vector4& rhs) const;
	Vector4 operator*(const float& scalar) const;
	Vector4 operator/(const float& scalar) const;

	Vector4& operator+=(const Vector4& rhs);
	Vector4& operator-=(const Vector4& rhs);
	Vector4& operator*=(const float& scalar);
	Vector4& operator/=(const float& scalar);
};

