#include "Math.h"

float Math::Clamp(const float& val, const float& min, const float& max)
{
	if (val < min)
		return min;
	else if (val > max)
		return max;
	else
		return val;
}
