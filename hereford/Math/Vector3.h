#pragma once

struct Vector3
{
	float mX;
	float mY;
	float mZ;


	Vector3();
	~Vector3() {};
	explicit Vector3(float x, float y, float z);
	explicit Vector3(int x, int y, int z);
	explicit Vector3(double x, double y, double z);
	Vector3(const Vector3& rhs);
	//Vector3(const class Vector2& rhs);

	float Magnitude() const;
	float SqrMagnitude() const;
	Vector3 normalized() const;
	void Normalize();

	float Dot(const Vector3& rhs) const;
	Vector3 Cross(const Vector3& rhs) const;

	static float Distance(const Vector3& lhs, const Vector3& rhs);
	static float AngleAsDeg(const Vector3& lhs, const Vector3& rhs);
	static float AngleAsRad(const Vector3& lhs, const Vector3& rhs);

	static const Vector3 Zero;
	static const Vector3 One;
	static const Vector3 Up;
	static const Vector3 Down;
	static const Vector3 Left;
	static const Vector3 Right;
	static const Vector3 Forward;
	static const Vector3 Back;



	Vector3& operator=(const Vector3& rhs);
	bool operator==(const Vector3* rhs) const;
	Vector3 operator+(const Vector3& rhs) const;
	Vector3 operator-(const Vector3& rhs) const;
	Vector3 operator*(const float& scalar) const;
	Vector3 operator/(const float& scalar) const;

	Vector3& operator+=(const Vector3& rhs);
	Vector3& operator-=(const Vector3& rhs);
	Vector3& operator*=(const float& scalar);
	Vector3& operator/=(const float& scalar);
};

