#include "Random.h"
#include<random>

std::random_device rd{};
std::mt19937 generator{ rd() };

float Random::UniformDistribution(float min, float max)
{
	std::uniform_real_distribution<float> distribution(min, max);
	return distribution(generator);
}

bool Random::Probability(float probability)
{
	return UnitInterval() <= probability;
}

float Random::UnitInterval()
{
	return UniformDistribution(0, 1);
}

float Random::NormalDistribution(float mean, float stdDev)
{
	std::normal_distribution<float> distribution(mean, stdDev);
	return distribution(generator);
}
