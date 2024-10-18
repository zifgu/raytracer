#include "test_common.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest {
    void assertFuzzyEqual(const Interval& expected, const Interval& actual, float tolerance) {
        Assert::AreEqual(expected.min(), actual.min(), tolerance, L"min");
        Assert::AreEqual(expected.max(), actual.max(), tolerance, L"max");
    }
    void assertFuzzyEqual(const glm::vec2& expected, const glm::vec2& actual, float tolerance) {
        Assert::AreEqual(expected.x, actual.x, tolerance, L"x");
        Assert::AreEqual(expected.y, actual.y, tolerance, L"y");
    }
    void assertFuzzyEqual(const glm::vec3& expected, const glm::vec3& actual, float tolerance) {
        Assert::AreEqual(expected.x, actual.x, tolerance, L"x");
        Assert::AreEqual(expected.y, actual.y, tolerance, L"y");
        Assert::AreEqual(expected.z, actual.z, tolerance, L"z");
    }
    void assertFuzzyEqual(const glm::vec4& expected, const glm::vec4& actual, float tolerance) {
        Assert::AreEqual(expected.x, actual.x, tolerance, L"x");
        Assert::AreEqual(expected.y, actual.y, tolerance, L"y");
        Assert::AreEqual(expected.z, actual.z, tolerance, L"z");
        Assert::AreEqual(expected.w, actual.w, tolerance, L"w");
    }
}