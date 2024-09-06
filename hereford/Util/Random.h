#pragma once

#include <random>


class Random
{
public:
	inline static float Range(float minInclusive, float maxExclusive)
	{
		std::random_device rd;
		std::seed_seq sd{ rd(), rd(), rd(), rd() };
		std::mt19937 gen(sd);
		std::uniform_real_distribution<> dis(minInclusive, maxExclusive);

		return dis(gen);
	}

};

