#pragma once

struct Quaternion
{
	float mX;
	float mY;
	float mZ;
	float mW;

	Quaternion();
	~Quaternion() {};
	explicit Quaternion(float x, float y, float z, float w);
	explicit Quaternion(float x);
	Quaternion(const Quaternion& rhs);

	Quaternion normalized() const;
	void Normalize();

	static const Quaternion Identity;

	Quaternion& operator=(const Quaternion& rhs);
	bool operator==(const Quaternion& rhs) const;
	bool operator!=(const Quaternion& rhs) const;
	float operator[](const int& index) const;

};

