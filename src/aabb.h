#pragma once

#include "common.h"
#include "ray.h"
#include "interval.h"

class AABox {
private:
	Interval hitInterval(int comp, const Ray& ray) const {
		const Interval& interval = getInterval(comp);
		float t1 = (interval.min - ray.origin()[comp]) / ray.direction()[comp];
		float t2 = (interval.max - ray.origin()[comp]) / ray.direction()[comp];
		return Interval(glm::min(t1, t2), glm::max(t1, t2));
	}
public:
	Interval x, y, z;
	
	AABox() {}
	AABox(const Interval& x, const Interval& y, const Interval& z) : x(x), y(y), z(z) {}
	AABox(const glm::vec3& a, const glm::vec3& b) {
		for (int comp = 0; comp < 3; ++comp) {
			Interval& interval = getInterval(comp);
			interval.min = glm::min(a[comp], b[comp]);
			interval.max = glm::max(a[comp], b[comp]);
		}
	}

	Interval& getInterval(int comp) {
		return comp == 0 ? x : (comp == 1 ? y : z);
	}
	const Interval& getInterval(int comp) const {
		return comp == 0 ? x : (comp == 1 ? y : z);
	}

	int longestAxis() const {
		if (x.size() > y.size())
			return x.size() > z.size() ? 0 : 2;
		else
			return y.size() > z.size() ? 1 : 2;
	}

	void expand(const AABox& other) {
		for (int comp = 0; comp < 3; ++comp) {
			getInterval(comp).expand(other.getInterval(comp));
		}
	}

	bool hit(const Ray& ray, Interval tRange) const {
		for (int comp = 0; comp < 3; ++comp) {
			Interval hitRange = hitInterval(comp, ray);
			if (hitRange.min > tRange.min) tRange.min = hitRange.min;
			if (hitRange.max < tRange.max) tRange.max = hitRange.max;

			if (tRange.min >= tRange.max) return false;
		}
		return true;
	}

	static AABox empty() { return AABox(Interval::empty(), Interval::empty(), Interval::empty()); }
	static AABox all() { return AABox(Interval::all(), Interval::all(), Interval::all()); }
};