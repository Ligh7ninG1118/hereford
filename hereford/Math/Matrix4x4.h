#pragma once

#include "Math.h"

struct Matrix4x4
{
	float m[4][4];

	Matrix4x4();
	~Matrix4x4();
	Matrix4x4(float m11, float m12, float m13, float m14,
		float m21, float m22, float m23, float m24,
		float m31, float m32, float m33, float m34,
		float m41, float m42, float m43, float m44);
	Matrix4x4(float x);
	Matrix4x4(const Matrix4x4& rhs);


	static const Matrix4x4 Zero;
	static const Matrix4x4 Identity;

	Matrix4x4& Translate(Vector3 translate);
	Matrix4x4& Translate(float x, float y, float z);
	Matrix4x4& Rotate(float x, float y, float z);
	Matrix4x4& Rotate(Vector3 eulerRot);
	Matrix4x4& Rotate(struct Quaternion quat);
	Matrix4x4& Scale(Vector3 scale);
	Matrix4x4& Scale(float scale);

	static Matrix4x4& CalculatePerspMatrix(float horFOV, float screenRatio, float nearPlane, float farPlane);
	static Matrix4x4& CalculateOrthoMatrix(float left, float right, float bottom, float top);



	Matrix4x4& Clear();

	Matrix4x4& operator=(const Matrix4x4& rhs);
	bool operator==(const Matrix4x4& rhs) const;
	bool operator!=(const Matrix4x4& rhs) const;
	Matrix4x4 operator*(const Matrix4x4& rhs) const;
	//Vec4 operator[]
};

