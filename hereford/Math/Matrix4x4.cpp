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

Matrix4x4::Matrix4x4(float x)
	:
	m{ x, 0.0f, 0.0f, 0.0f,
		0.0f, x, 0.0f, 0.0f,
		0.0f, 0.0f, x, 0.0f,
		0.0f, 0.0f, 0.0f, x, }
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

Matrix4x4& Matrix4x4::Translate(Vector3 translate)
{
	// column major? switch to [i][3]
	for (int i = 0; i < 3; i++)
		m[3][i] += translate[i];
	return *this;
}

Matrix4x4& Matrix4x4::Translate(float x, float y, float z)
{
	m[3][0] += x;
	m[3][1] += y;
	m[3][2] += z;
	return *this;
}


// Deprecated
//Matrix4x4& Matrix4x4::Rotate(const float& radAngle, const Vector3& axis)
//{
//	Vector3 axisNorm = axis.normalized();
//	float cosTheta = cos(radAngle);
//	float sinTheta = sin(radAngle);
//
//	Matrix4x4 rot = Matrix4x4::Identity;
//	Vector3 temp = axisNorm * (1 - cosTheta);
//
//	rot.m[0][0] = cosTheta + temp[0] * axisNorm[0]; 
//	rot.m[0][1] = temp[0] * axisNorm[1] + sinTheta * axisNorm[2];
//	rot.m[0][2] = temp[0];
//
//	rot.m[0][0] = cosTheta + temp[0] * axisNorm[0];
//	rot.m[0][1] = temp[0] * axisNorm[1] + sinTheta * axisNorm[2];
//	rot.m[0][2] = temp[0] * axisNorm[2] - sinTheta * axisNorm[1];
//
//	rot.m[1][0] = temp[1] * axisNorm[0] - sinTheta * axisNorm[2];
//	rot.m[1][1] = cosTheta + temp[1] * axisNorm[1];
//	rot.m[1][2] = temp[1] * axisNorm[2] + sinTheta * axisNorm[0];
//
//	rot.m[2][0] = temp[2] * axisNorm[0] + sinTheta * axisNorm[1];
//	rot.m[2][1] = temp[2] * axisNorm[1] - sinTheta * axisNorm[0];
//	rot.m[2][2] = cosTheta + temp[2] * axisNorm[2];
//
//	*this = *this * rot;
//
//	return *this;
//}

//In degree angles
Matrix4x4& Matrix4x4::Rotate(float x, float y, float z)
{
	Quat quat = Quaternion::EulerToQuat(Vec3(x, y, z));
	return Rotate(quat);
}

//In degree angles
Matrix4x4& Matrix4x4::Rotate(Vector3 eulerRot)
{
	Quat quat = Quaternion::EulerToQuat(eulerRot);
	return Rotate(quat);
}

Matrix4x4& Matrix4x4::Rotate(Quaternion quat)
{
	Matrix4x4 rot = Matrix4x4::Identity;

	float q0sqr = quat.mX * quat.mX;
	float q1sqr = quat.mY * quat.mY;
	float q2sqr = quat.mZ * quat.mZ;
	float q3sqr = quat.mW * quat.mW;

	float q0 = quat.mX;
	float q1 = quat.mY;
	float q2 = quat.mZ;
	float q3 = quat.mW;


	rot.m[0][0] = 1.0f - 2.0f * (q1sqr + q2sqr);
	rot.m[1][0] = 2.0f * (q0 * q1 - q2 * q3);
	rot.m[2][0] = 2.0f * (q0 * q2 + q1 * q3);
	rot.m[3][0] = 0.0f;

	rot.m[0][1] = 2.0f * (q0 * q1 + q2 * q3);
	rot.m[1][1] = 1.0f - 2.0f * (q0sqr + q2sqr);
	rot.m[2][1] = 2.0f * (q1 * q2 - q0 * q3);
	rot.m[3][1] = 0.0f;

	rot.m[0][2] = 2.0f * (q0 * q2 - q1 * q3);
	rot.m[1][2] = 2.0f * (q1 * q2 + q0 * q3);
	rot.m[2][2] = 1.0f - 2.0f * (q0sqr + q1sqr);
	rot.m[3][2] = 0.0f;

	rot.m[0][3] = rot.m[1][3] = rot.m[2][3] = 0.0f;
	rot.m[3][3] = 1.0f;

	*this = *this * rot;

	return *this;
}

Matrix4x4& Matrix4x4::Scale(Vector3 scale)
{
	for (int i = 0; i < 3; i++)
		m[i][i] *= scale[i];
	return *this;
}

Matrix4x4& Matrix4x4::Scale(float scale)
{
	for (int i = 0; i < 3; i++)
		m[i][i] *= scale;
	return *this;
}

Matrix4x4 Matrix4x4::CalculatePerspMatrix(float verFOV, float screenRatio, float nearPlane, float farPlane)
{
	Mat4 projection = Mat4::Zero;

	float tanHalfFOV = tan(DEG2RAD * verFOV / 2.0f);
	projection.m[0][0] = 1.0f / tanHalfFOV;
	projection.m[1][1] = 1.0f * screenRatio / tanHalfFOV;
	projection.m[2][2] = -(farPlane + nearPlane) / (farPlane - nearPlane);
	projection.m[2][3] = -1.0f;
	projection.m[3][2] = -(2.0f * farPlane * nearPlane) / (farPlane - nearPlane);

	return projection;
}

Matrix4x4 Matrix4x4::CalculateOrthoMatrix(float left, float right, float bottom, float top)
{
	Mat4 projection = Mat4::Identity;
	projection.m[0][0] = 2.0f / (right - left);
	projection.m[1][1] = 2.0f / (top - bottom);
	projection.m[2][2] = -1.0f;

	projection.m[3][0] = -(right + left) / (right - left);
	projection.m[3][1] = -(top + bottom) / (top - bottom);
	projection.m[3][2] = 0.0f;

	return projection;
}

Matrix4x4 Matrix4x4::CalculateLookAtMatrix(Vector3 front, Vector3 right, Vector3 up)
{
	Mat4 MLookAt = Mat4::Identity;

	front.Normalize();
	right.Normalize();
	up.Normalize();

	MLookAt.m[0][0] = right.mX;		MLookAt.m[1][0] = right.mY;		MLookAt.m[2][0] = right.mZ;		MLookAt.m[3][0] = 0.0f;
	MLookAt.m[0][1] = up.mX;		MLookAt.m[1][1] = up.mY;		MLookAt.m[2][1] = up.mZ;		MLookAt.m[3][1] = 0.0f;
	MLookAt.m[0][2] = -front.mX;	MLookAt.m[1][2] = -front.mY;	MLookAt.m[2][2] = -front.mZ;	MLookAt.m[3][2] = 0.0f;
	MLookAt.m[0][3] = 0.0f;			MLookAt.m[1][3] = 0.0f;			MLookAt.m[2][3] = 0.0f;			MLookAt.m[3][3] = 1.0f;

	return MLookAt;
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
			if (abs(m[i][j] - rhs.m[i][j]) > EPISILON)
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
