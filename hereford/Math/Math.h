#pragma once

#include <cmath>
#include <limits>
#include "Vector3.h"

const float PI = 3.14159265358f;
const float EPSILON = 0.00001f;

const float DEG2RAD = (PI * 2.0f) / 360.0f;
const float RAD2DEG = 360.0f / (PI * 2.0f);

const float POS_INFINITY = std::numeric_limits<float>::infinity();
const float NEG_INFINITY = std::numeric_limits<float>::infinity();

const Vector3 Vector3::Zero(0.0f, 0.0f, 0.0f);
const Vector3 Vector3::Up;
const Vector3 Vector3::Down;
const Vector3 Vector3::Left;
const Vector3 Vector3::Right;
const Vector3 Vector3::Forward;
const Vector3 Vector3::Back;


class Math
{
};

