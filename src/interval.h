#pragma once

#include "common.h"

class Interval {
	// Interval is empty by default
	float m_min = infinity;
	float m_max = -infinity;
public:
	Interval() {}
	Interval(float min, float max) : m_min(min), m_max(max) {}

	float min() const { return m_min; }
	float max() const { return m_max; }
	void setMin(float min) { m_min = min; }
	void setMax(float max) { m_max = max; }

	float size() const { return max() - min(); }
	bool contains(float x) const { return min() <= x && x <= max(); }
	bool surrounds(float x) const { return min() < x && x < max(); }
	float clamp(float x) const { return glm::clamp(x, min(), max()); }

	void expand(const Interval& other) {
		m_min = glm::min(min(), other.min());
		m_max = glm::max(max(), other.max());
	}

	static const Interval empty;
	static const Interval positive;
	static const Interval all;
};
