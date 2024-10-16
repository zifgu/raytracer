#pragma once

#include <vector>
#include <memory>

#include "hittable.h"

class HittableList : public Hittable {
	std::vector<std::shared_ptr<Hittable>> objects;
	AABox bbox = AABox::empty();
public:
	void clear() { objects.clear(); }
	void add(std::shared_ptr<Hittable> object) {
		objects.push_back(object);
		bbox.expand(object->boundingBox());
	}
	const std::vector<std::shared_ptr<Hittable>>& get() const { return objects; }
	bool hit(const Ray& ray, Interval tRange, HitRecord& hit) const override {
		bool hitAnything = false;
		float closestT = tRange.max;
		HitRecord tempHit;

		for (const std::shared_ptr<Hittable>& object : objects) {
			if (object->hit(ray, Interval(tRange.min, closestT), tempHit)) {
				hitAnything = true;
				closestT = tempHit.t;
				hit = tempHit;
			}
		}

		return hitAnything;
	}

	AABox boundingBox() const override { return bbox; }
};