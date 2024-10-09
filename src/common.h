#pragma once

#include <limits>
#include <random>

const float infinity = std::numeric_limits<float>::infinity();

inline float random() {
	static std::uniform_real_distribution<float> distribution(0.0, 1.0);
	static std::mt19937 generator;
	return distribution(generator);
}

inline float random(float min, float max) {
	return min + (max - min) * random();
}