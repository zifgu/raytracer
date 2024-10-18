#pragma once

#include "CppUnitTest.h"

#include "../src/interval.h"
#include "../src/aabb.h"
#include "../src/ray.h"

// Display strings
namespace Microsoft
{
    namespace VisualStudio
    {
        namespace CppUnitTestFramework
        {
            template<> static std::wstring ToString<glm::vec2>(const glm::vec2& t) {
                std::wstringstream wss;
                wss << "{x=" << t.x << " y=" << t.y << "}";
                return wss.str();
            }

            template<> static std::wstring ToString<glm::vec3>(const glm::vec3& t) { 
                std::wstringstream wss;
                wss << "{x=" << t.x << " y=" << t.y << " z=" << t.z << "}";
                return wss.str();
            }

            template<> static std::wstring ToString<glm::vec4>(const glm::vec4& t) {
                std::wstringstream wss;
                wss << "{x=" << t.x << " y=" << t.y << " z=" << t.z << " w=" << t.w << "}";
                return wss.str();
            }

            template<> static std::wstring ToString<Interval>(const Interval& t) {
                std::wstringstream wss;
                wss << "{min=" << t.min() << " max=" << t.max() << "}";
                return wss.str();
            }

            template<> static std::wstring ToString<Ray>(const Ray& t) {
                std::wstringstream wss;
                wss << "{or=" << ToString(t.origin()) << " dir=" << ToString(t.direction()) << "}";
                return wss.str();
            }

            template<> static std::wstring ToString<AABox>(const AABox& t) {
                std::wstringstream wss;
                wss << "{x=" << ToString(t.x()) << " y=" << ToString(t.y()) << " z=" << ToString(t.z()) << "}";
                return wss.str();
            }
        }
    }
}

// Utility functions
namespace UnitTest {
    template<typename T>
    void testEqualityHelper(const T& a, const T& b, bool expectEqual)
    {
        if (expectEqual) {
            Assert::IsTrue(a == b);
            Assert::IsFalse(a != b);
            Assert::IsTrue(b == a);
            Assert::IsFalse(b != a);
        }
        else {
            Assert::IsFalse(a == b);
            Assert::IsTrue(a != b);
            Assert::IsFalse(b == a);
            Assert::IsTrue(b != a);
        }
    }

    template<typename T>
    void assertSharedPtrEqual(const std::shared_ptr<T> a, const std::shared_ptr<T> b) {
        if (a == nullptr || b == nullptr) {
            Assert::IsTrue(a == nullptr && b == nullptr);
        }
        else {
            Assert::AreEqual(static_cast<void*>(a.get()), static_cast<void*>(b.get()));
        }
    }

    void assertFuzzyEqual(const Interval& expected, const Interval& actual, float tolerance);
    void assertFuzzyEqual(const glm::vec2& expected, const glm::vec2& actual, float tolerance);
    void assertFuzzyEqual(const glm::vec3& expected, const glm::vec3& actual, float tolerance);
    void assertFuzzyEqual(const glm::vec4& expected, const glm::vec4& actual, float tolerance);
}