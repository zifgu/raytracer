#pragma once

#include <glm/common.hpp>

#include "common.h"

class Interval {
public:
	float min = -infinity;
	float max = infinity;

	Interval() {}
	Interval(float min, float max) : min(min), max(max) {}

	float size() const { return max - min; }
	bool contains(float x) const { return min <= x && x <= max; }
	bool surrounds(float x) const { return min < x && x < max; }
	float clamp(float x) const { return glm::clamp(x, min, max); }

	static Interval empty() { return { infinity, -infinity }; }
	static Interval positive() { return { 0.f, infinity }; }
	static Interval all() { return { -infinity, infinity }; }
};
