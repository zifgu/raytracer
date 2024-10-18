#pragma once

#include "CppUnitTest.h"

#include "test_common.h"
#include "../src/hittable.h"

// Utility functions for testing Hittables

namespace UnitTest {
    // asserts that two HitRecords are equal, within tolerance
    void assertHitEqual(const Hittable::HitRecord& expectHit, const Hittable::HitRecord& hit, float tolerance);

    // asserts that the ray does not hit (and hit method does not modify the HitRecord)
    void assertMiss(const Hittable& hittable, const Ray& ray, const Interval& tRange);

    // asserts that the ray hits, and the HitRecord matches expected
    void assertHit(const Hittable& hittable, const Ray& ray, const Interval& tRange, const Hittable::HitRecord& expectHit, float tolerance);

    // varies the t range for a hit at a given point
    // assumes:
    // - expectT, safeTDist are positive
    // - expectT > safeTDist
    // - no other intersections exist within +- safeTDist of expectT
    void testHitTRange(const Hittable& hittable, const Ray& ray, const Hittable::HitRecord& expectHit, float expectT, float safeTDist, float tolerance);

    // varies the ray scale and t range for a hit at a given point
    // assumes:
    // - expectT, safeTDist are positive
    // - expectT > safeTDist
    // - no other intersections exist within +- safeTDist of expectT
    void testHitTScalesAndTRange(const Hittable& hittable, const Ray& ray, const Hittable::HitRecord& expectHit, float expectT, float safeTDist, float tolerance);
}