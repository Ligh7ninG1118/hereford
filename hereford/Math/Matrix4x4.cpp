#include "Math.h"
#include "Matrix4x4.h"

const Matrix4x4 Matrix4x4::Zero;
const Matrix4x4 Matrix4x4::Identity(1.0f, 0.0f, 0.0f, 0.0f,
									0.0f, 1.0f, 0.0f, 0.0f,
									0.0f, 0.0f, 1.0f, 0.0f,
									0.0f, 0.0f, 0.0f, 1.0f);

Matrix4x4::Matrix4x4()
	:
	m{ 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f }
{
}

Matrix4x4::~Matrix4x4()
{
}

Matrix4x4::Matrix4x4(float m11, float m12, float m13, float m14,
					float m21, float m22, float m23, float m24, 
					float m31, float m32, float m33, float m34, 
					float m41, float m42, float m43, float m44)
	:
	m{ m11, m12, m13, m14, 
		m21, m22, m23, m24, 
		m31, m32, m33, m34, 
		m41, m42, m43, m44, }
{
}



Matrix4x4::Matrix4x4(const Matrix4x4& rhs)
	:
	m{ rhs.m[0][0], rhs.m[0][1], rhs.m[0][2], rhs.m[0][3],
		rhs.m[1][0], rhs.m[1][1], rhs.m[1][2], rhs.m[1][3], 
		rhs.m[2][0], rhs.m[2][1], rhs.m[2][2], rhs.m[2][3], 
		rhs.m[3][0], rhs.m[3][1], rhs.m[3][2], rhs.m[3][3], }
{
}

Matrix4x4& Matrix4x4::Translate(const Vector3& translate)
{
	for (int i = 0; i < 3; i++)
		m[3][i] += translate[i];
	return *this;
}

Matrix4x4& Matrix4x4::Rotate(const float& radAngle, const Vector3& axis)
{
	Vector3 axisNorm = axis.normalized();
	float cosTheta = cos(radAngle);
	float sinTheta = sin(radAngle);

	Matrix4x4 rot = Matrix4x4::Identity;
	Vector3 temp = axisNorm * (1 - cosTheta);

	rot.m[0][0] = cosTheta + temp[0] * axisNorm[0]; 
	rot.m[0][1] = temp[0] * axisNorm[1] + sinTheta * axisNorm[2];
	rot.m[0][2] = temp[0];

	rot.m[0][0] = cosTheta + temp[0] * axisNorm[0];
	rot.m[0][1] = temp[0] * axisNorm[1] + sinTheta * axisNorm[2];
	rot.m[0][2] = temp[0] * axisNorm[2] - sinTheta * axisNorm[1];

	rot.m[1][0] = temp[1] * axisNorm[0] - sinTheta * axisNorm[2];
	rot.m[1][1] = cosTheta + temp[1] * axisNorm[1];
	rot.m[1][2] = temp[1] * axisNorm[2] + sinTheta * axisNorm[0];

	rot.m[2][0] = temp[2] * axisNorm[0] + sinTheta * axisNorm[1];
	rot.m[2][1] = temp[2] * axisNorm[1] - sinTheta * axisNorm[0];
	rot.m[2][2] = cosTheta + temp[2] * axisNorm[2];

	*this = *this * rot;

	return *this;
}

Matrix4x4& Matrix4x4::Scale(const Vector3& scale)
{
	for (int i = 0; i < 3; i++)
		m[i][i] *= scale[i];
	return *this;
}

Matrix4x4& Matrix4x4::Clear()
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			m[i][j] = 0.0f;
	return *this;
}

Matrix4x4& Matrix4x4::operator=(const Matrix4x4& rhs)
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			m[i][j] = rhs.m[i][j];
	return *this;
}

bool Matrix4x4::operator==(const Matrix4x4& rhs) const
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			if (abs(m[i][j] - rhs.m[i][j]) > EPSILON)
				return false;
	return true;
}

bool Matrix4x4::operator!=(const Matrix4x4& rhs) const
{
	return !(*this == rhs);
}

Matrix4x4 Matrix4x4::operator*(const Matrix4x4& rhs) const
{
	Matrix4x4 result = Matrix4x4::Zero;

	for(int i=0;i<4;i++)
		for (int j = 0; j < 4; j++)
			for (int k = 0; k < 4; k++)
				result.m[j][i] += m[k][i] * rhs.m[j][k];

	return result;
}
