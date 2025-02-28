#ifndef TREEGENERATOR_RANDOM_H
#define TREEGENERATOR_RANDOM_H

class Random {
public:
	// Returns a random sample from a uniform distribution between min and max
	static float UniformDistribution(float min, float max);

	// Randomly returns a bool based on the inputted probability
	static bool Probability(float probability);

	// Randomly returns a value in the interval [0,1)
	static float UnitInterval();

	// Randomly returns a value using a normal distribution with the inputted mean and standard deviation
	static float NormalDistribution(float mean, float stdDev);
};

#endif //TREEGENERATOR_RANDOM_H
