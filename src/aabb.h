#pragma once

#include "common.h"

class AABox {
	Interval m_x = Interval::empty;
	Interval m_y = Interval::empty;
	Interval m_z = Interval::empty;

	Interval hitInterval(int comp, const Ray& ray) const {
		const Interval& interval = getInterval(comp);
		float t1 = (interval.min() - ray.origin()[comp]) / ray.direction()[comp];
		float t2 = (interval.max() - ray.origin()[comp]) / ray.direction()[comp];
		return Interval(glm::min(t1, t2), glm::max(t1, t2));
	}
public:
	AABox() {}
	AABox(const Interval& x, const Interval& y, const Interval& z) : m_x(x), m_y(y), m_z(z) {}
	AABox(const glm::vec3& a, const glm::vec3& b) {
		for (int comp = 0; comp < 3; ++comp) {
			Interval& interval = getInterval(comp);
			interval.setMin(glm::min(a[comp], b[comp]));
			interval.setMax(glm::max(a[comp], b[comp]));
		}
	}

	Interval& x() { return m_x; }
	Interval& y() { return m_y; }
	Interval& z() { return m_z; }
	const Interval& x() const { return m_x; }
	const Interval& y() const { return m_y; }
	const Interval& z() const { return m_z; }

	Interval& getInterval(int comp) {
		return comp == 0 ? m_x : (comp == 1 ? m_y : m_z);
	}
	const Interval& getInterval(int comp) const {
		return comp == 0 ? m_x : (comp == 1 ? m_y : m_z);
	}

	int longestAxis() const {
		if (m_x.size() > m_y.size())
			return m_x.size() > m_z.size() ? 0 : 2;
		else
			return m_y.size() > m_z.size() ? 1 : 2;
	}

	void expand(const AABox& other) {
		for (int comp = 0; comp < 3; ++comp) {
			getInterval(comp).expand(other.getInterval(comp));
		}
	}
	void expand(const glm::vec3& point) {
		for (int comp = 0; comp < 3; ++comp) {
			getInterval(comp).expand(point[comp]);
		}
	}

	bool hit(const Ray& ray, Interval tRange) const {
		for (int comp = 0; comp < 3; ++comp) {
			Interval hitRange = hitInterval(comp, ray);
			if (hitRange.min() > tRange.min()) tRange.setMin(hitRange.min());
			if (hitRange.max() < tRange.max()) tRange.setMax(hitRange.max());

			if (tRange.min() >= tRange.max()) return false;
		}
		return true;
	}

	static const AABox empty;
	static const AABox all;
};
