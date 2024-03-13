#include "Random.h"

#include <random>

float Random::Range(float minInclusive, float maxExclusive)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(minInclusive, maxExclusive);

	return dis(gen);
}
