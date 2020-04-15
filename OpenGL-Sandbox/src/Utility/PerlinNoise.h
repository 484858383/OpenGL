#pragma once

#include<cstdint>

struct NoiseData
{
	float persistence = 0.34f; ///height variance
	float smooth = 75.f; ///how smooth
	float frequency = 2.f; ///rate of change of height
	int octaves = 8; ///detail
	int ampltude = 255; ///bonus height
	uint32_t seed = 0;
};

class PerlinNoise
{
public:
	PerlinNoise();
	PerlinNoise(const NoiseData& nd);
	PerlinNoise(float persistence, float smooth, float frequency, int octaves, int ampltude, uint32_t seed);

	double Noise2D(float x, float y);
	void setNoiseFunction(const NoiseData& nd);
	void setNoiseFunction(float persistence, float smooth, float frequency, int octaves, int ampltude, uint32_t seed);
private:
	double cosInterpolate(double a, double b, double x);

	double getNoise(int n);
	double getNoise(int x, int y);
	double smoothNoise1D(double x);
	double smoothNoise2D(double x, double y);

	double interpolatedNoise(float x, float y);
private:
	NoiseData m_noiseData;
};