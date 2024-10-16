#pragma once

#include "common.h"
#include "hittable.h"

class BVHNode : public Hittable {
	AABox m_bbox = AABox::empty;
	std::shared_ptr<Hittable> m_left = nullptr;
	std::shared_ptr<Hittable> m_right = nullptr;

	static bool aabbCompare(std::shared_ptr<Hittable> a, std::shared_ptr<Hittable> b, int comp) {
		return a->boundingBox().getInterval(comp).min() < b->boundingBox().getInterval(comp).min();
	}
	static bool aabbCompareX(std::shared_ptr<Hittable> a, std::shared_ptr<Hittable> b) {
		return aabbCompare(a, b, 0);
	}
	static bool aabbCompareY(std::shared_ptr<Hittable> a, std::shared_ptr<Hittable> b) {
		return aabbCompare(a, b, 1);
	}
	static bool aabbCompareZ(std::shared_ptr<Hittable> a, std::shared_ptr<Hittable> b) {
		return aabbCompare(a, b, 2);
	}
public:
	BVHNode(std::vector<std::shared_ptr<Hittable>> objects, size_t start, size_t end) {
		for (const std::shared_ptr<Hittable>& obj : objects) {
			m_bbox.expand(obj->boundingBox());
		}

		size_t range = end - start;
		if (range == 1) {
			m_left = objects[start];
		}
		else if (range == 2) {
			m_left = objects[start];
			m_right = objects[start + 1];
		}
		else {
			int axis = m_bbox.longestAxis();
			auto comparator = axis == 0 ? &aabbCompareX : (axis == 1 ? &aabbCompareY : &aabbCompareZ);
			std::sort(objects.begin() + start, objects.begin() + end, comparator);

			size_t mid = start + range / 2;
			m_left = std::make_shared<BVHNode>(objects, start, mid);
			m_right = std::make_shared<BVHNode>(objects, mid, end);
		}
	}

	bool hit(const Ray& ray, Interval tRange, HitRecord& hit) const override {
		if (!m_bbox.hit(ray, tRange)) return false;

		bool leftHit = m_left != nullptr && m_left->hit(ray, tRange, hit);
		bool rightHit = m_right != nullptr && m_right->hit(ray, Interval(tRange.min(), leftHit ? hit.t : tRange.max()), hit);

		return leftHit || rightHit;
	}

	AABox boundingBox() const override { return m_bbox; }
};