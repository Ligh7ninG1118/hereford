#pragma once

struct IHittable
{
	IHittable(){}
	~IHittable(){}
	virtual void Hit(const struct HitInfo& info) = 0;
};