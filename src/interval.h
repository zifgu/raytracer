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

	inline bool operator==(const Interval& other) const { return m_min == other.m_min && m_max == other.m_max; }
	inline bool operator!=(const Interval& other) const { return !(*this == other); }

	float size() const { return max() - min(); }
	bool contains(float x) const { return min() <= x && x <= max(); }
	bool surrounds(float x) const { return min() < x && x < max(); }
	float clamp(float x) const { return glm::clamp(x, min(), max()); }

	void expand(const Interval& other) {
		m_min = glm::min(min(), other.min());
		m_max = glm::max(max(), other.max());
	}
	void expand(float x) { 
		m_min = glm::min(min(), x);
		m_max = glm::max(max(), x);
	}
	void pad(float delta) {
		m_min = min() - 0.5f * delta;
		m_max = max() + 0.5f * delta;
	}

	static const Interval empty;
	static const Interval positive;
	static const Interval negative;
	static const Interval all;
};
