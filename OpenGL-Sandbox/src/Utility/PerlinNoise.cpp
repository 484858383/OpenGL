#include"PerlinNoise.h"

#include<cmath>

PerlinNoise::PerlinNoise()
{
}

PerlinNoise::PerlinNoise(const NoiseData & nd)
	:m_noiseData(nd)
{
}

PerlinNoise::PerlinNoise(float persistence, float smooth, float frequency, int octaves, int ampltude, uint32_t seed)
	:m_noiseData({persistence, smooth, frequency, octaves, ampltude, seed})
{
}

double PerlinNoise::Noise2D(float x, float y)
{
	double total = 0;

	for(int i = 0; i < m_noiseData.octaves; i++)
	{
		double freq = std::pow(m_noiseData.frequency, i);
		double amplitude = std::pow(m_noiseData.persistence, i);

		total += interpolatedNoise(x * freq / m_noiseData.smooth, y * freq / m_noiseData.smooth) * amplitude;
	}
	return std::fmax(1, total + m_noiseData.ampltude);
}

void PerlinNoise::setNoiseFunction(const NoiseData& nd)
{
	m_noiseData = nd;
}

void PerlinNoise::setNoiseFunction(float persistence, float smooth, float frequency, int octaves, int ampltude, uint32_t seed)
{
	m_noiseData = {persistence, smooth, frequency, octaves, ampltude};
}

double PerlinNoise::getNoise(int n)
{
	n = (n << 13) ^ n;
	return 1.f - (((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.f);
}

double PerlinNoise::getNoise(int x, int y)
{
	int n = x + y * (57 + m_noiseData.seed);
	n = (n << 13) ^ n;
	return 1.0 - (((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
}

double PerlinNoise::smoothNoise1D(double x)
{
	return getNoise(x) / 2.f + getNoise(x - 1) / 4.f + getNoise(x + 1) / 4.f;
}

double PerlinNoise::smoothNoise2D(double x, double y)
{
	double corners = (getNoise(x - 1, y - 1) + getNoise(x + 1, y - 1) + getNoise(x - 1, y + 1) + getNoise(x + 1, y + 1)) / 16.f;
	double sides   = (getNoise(x - 1, y)     + getNoise(x + 1, y)     + getNoise(x,     y - 1) + getNoise(x,     y + 1)) / 8.f;
	double center  =  getNoise(x, y) / 4.f;

	return corners + sides + center;
}

double PerlinNoise::interpolatedNoise(float x, float y)
{
	int X = std::floor(x);
	int Y = std::floor(y);

	//get smooth values around point
	double v0 = smoothNoise2D(X, Y);
	double v1 = smoothNoise2D(X + 1, Y);
	double v2 = smoothNoise2D(X, Y + 1);
	double v3 = smoothNoise2D(X + 1, Y + 1);

	//interpolate values
	auto i0 = cosInterpolate(v0, v1, X - x);
	auto i1 = cosInterpolate(v2, v3, X - x);

	return cosInterpolate(i0, i1, Y - y);
}

double PerlinNoise::cosInterpolate(double a, double b, double x)
{
	double ft = x * 3.1415927; //pi
	double f = (1.f - std::cos(ft)) * 0.5f;

	return  a * (1.f - f) + b * f;
}