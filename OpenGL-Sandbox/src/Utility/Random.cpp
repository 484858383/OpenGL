#include "Random.h"

#include<chrono>

Random::Random()
	:m_seed(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()))
{
	m_rng.seed(m_seed);
}

int32_t Random::getIntInRangeImpl(int32_t low, int32_t high)
{
	std::uniform_int_distribution<int32_t> dist(low, high);
	return dist(m_rng);
}

double Random::getDoubleInRangeImpl(double low, double high)
{
	std::uniform_real_distribution<double> dist(low, high);
	return dist(m_rng);
}

bool Random::getBoolImpl()
{
	return getIntInRangeImpl(0, 1);
}
