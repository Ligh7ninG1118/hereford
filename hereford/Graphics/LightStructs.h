#pragma once
#include "Math/Math.h"

enum class LightType
{
	ReservedDefault = 0,

	DirLight,
	SpotLight,
	PointLight,
};


struct DirLight
{
	Vec3 direction;
	Vec3 ambient;
	Vec3 diffuse;
	Vec3 specular;
};

struct PointLight
{
	Vec3 ambient;
	Vec3 diffuse;
	Vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

struct SpotLight
{
	Vec3 direction;
	float innerCutoff;
	float outerCutoff;

	Vec3 ambient;
	Vec3 diffuse;
	Vec3 specular;

	float constant;
	float linear;
	float quadratic;
};