#pragma once

#include<random>
#include<cstdint>

class Random
{
public:
	Random(const Random&) = delete;
	Random& operator=(const Random&) = delete;

	Random(Random&&) = delete;
	Random& operator=(Random&&) = delete;

	~Random() = default;

	static Random& get() { static Random r; return r; }

	static int32_t getIntInRange(int32_t low, int32_t high) { return get().getIntInRangeImpl(low, high); }
	static double getDoubleInRange(double low, double high) { return get().getDoubleInRangeImpl(low, high); }
	static bool getBool() { return get().getBoolImpl(); }
	static int32_t getSeed() { return get().m_seed; }
private:
	Random();

	int32_t getIntInRangeImpl(int32_t low, int32_t high);
	double getDoubleInRangeImpl(double low, double high);
	bool getBoolImpl();
private:
	std::mt19937 m_rng;
	int32_t m_seed;
};