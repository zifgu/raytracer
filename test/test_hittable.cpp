#include "test_hittable.h"

#include "../src/lambertian.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest {

	void assertHitEqual(const Hittable::HitRecord& expectHit, const Hittable::HitRecord& hit, float tolerance) {
		Assert::AreEqual(expectHit.t, hit.t, tolerance);
		assertFuzzyEqual(expectHit.point, hit.point, tolerance);
		assertFuzzyEqual(expectHit.normal, hit.normal, tolerance);
		Assert::AreEqual(expectHit.frontFace, hit.frontFace);
		assertFuzzyEqual(expectHit.uv, hit.uv, tolerance);
		assertSharedPtrEqual(expectHit.material, hit.material);
	}

	void assertMiss(const Hittable& hittable, const Ray& ray, const Interval& tRange) {
		// set some nonsense hit data - expect it to be unchanged
		Hittable::HitRecord hit;
		hit.material = std::make_shared<Lambertian>(glm::vec3(-100.f));
		hit.normal = glm::vec3(-100.f);
		hit.point = glm::vec3(100.f);
		hit.uv = glm::vec2(100.f);
		hit.t = 100.f;
		hit.frontFace = true;

		const Hittable::HitRecord before = hit;
		Assert::IsFalse(hittable.hit(ray, tRange, hit));
		assertHitEqual(before, hit, 0.f);
	}

	void assertHit(const Hittable& hittable, const Ray& ray, const Interval& tRange, const Hittable::HitRecord& expectHit, float tolerance) {
		Hittable::HitRecord hit;
		Assert::IsTrue(hittable.hit(ray, tRange, hit));
		assertHitEqual(expectHit, hit, tolerance);
	}

	void testHitTRange(const Hittable& hittable, const Ray& ray, const Hittable::HitRecord& expectHit, float expectT, float safeTDist, float tolerance, bool expectTangent) {

		// test positive t
		{
			Hittable::HitRecord expectHitCopy = expectHit;
			expectHitCopy.t = expectT;

			assertHit(hittable, ray, Interval::positive, expectHitCopy, tolerance);
			assertHit(hittable, ray, Interval(expectT - safeTDist, expectT + safeTDist), expectHitCopy, tolerance);
			assertMiss(hittable, ray, Interval(0.f, expectT - safeTDist));
			assertMiss(hittable, ray, Interval(expectT + 0.5f * safeTDist, expectT + safeTDist));
		}

		// reverse the ray to test negative t
		{
			const Ray reversedRay(ray.origin(), -ray.direction());
			Hittable::HitRecord expectHitReversed = expectHit;
			// if ray is expected to be tangent to surface, reversing it should not affect normal
			if (!expectTangent) {
				expectHitReversed.normal = -expectHit.normal;	
				expectHitReversed.frontFace = !expectHit.frontFace;
			}
			expectHitReversed.t = -expectT;

			// do not test this case - if there is a second intersection with smaller (more negative) t, the method may return that intersection instead
			//assertHit(hittable, reversedRay, Interval::negative, expectHitReversed, tolerance);

			assertHit(hittable, reversedRay, Interval(-expectT - safeTDist, -expectT + safeTDist), expectHitReversed, tolerance);
			assertMiss(hittable, reversedRay, Interval(-expectT + safeTDist, 0.f));
			assertMiss(hittable, reversedRay, Interval(-expectT - safeTDist, -expectT - 0.5f * safeTDist));
		}

		// test with degenerate and reversed ranges
		{
			assertMiss(hittable, ray, Interval(expectT, expectT));
			assertMiss(hittable, ray, Interval(expectT + safeTDist, expectT + safeTDist));
			assertMiss(hittable, ray, Interval(expectT + safeTDist, expectT - safeTDist));
		}
	}

	void testHitTScalesAndTRange(const Hittable& hittable, const Ray& ray, const Hittable::HitRecord& expectHit, float expectT, float safeTDist, float tolerance) {
		testHitTRange(hittable, ray, expectHit, expectT, safeTDist, tolerance, false);
		testHitTRange(hittable, Ray(ray.origin(), ray.direction() * 0.1f), expectHit, expectT / 0.1f, safeTDist / 0.1f, tolerance, false);
		testHitTRange(hittable, Ray(ray.origin(), ray.direction() * 0.5f), expectHit, expectT / 0.5f, safeTDist / 0.5f, tolerance, false);
		testHitTRange(hittable, Ray(ray.origin(), ray.direction() * 3.f), expectHit, expectT / 3.f, safeTDist / 3.f, tolerance, false);
		testHitTRange(hittable, Ray(ray.origin(), ray.direction() * 10.f), expectHit, expectT / 10.f, safeTDist / 10.f, tolerance, false);
	}
}