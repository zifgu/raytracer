#include "pch.h"
#include "CppUnitTest.h"

#include "test_common.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	TEST_CLASS(TestAABB)
	{
	public:
		TEST_METHOD(TestConstructor)
		{
			// default
			{
				AABox box;
				Assert::AreEqual(infinity, box.x().min());
				Assert::AreEqual(-infinity, box.x().max());
				Assert::AreEqual(infinity, box.y().min());
				Assert::AreEqual(-infinity, box.y().max());
				Assert::AreEqual(infinity, box.z().min());
				Assert::AreEqual(-infinity, box.z().max());
			}

			// with args
			{
				Interval x(-1.f, 1.f);
				Interval y(-2.f, -1.f);
				Interval z(0.f, 2.f);
				AABox box(x, y, z);
				Assert::AreEqual(x.min(), box.x().min());
				Assert::AreEqual(x.max(), box.x().max());
				Assert::AreEqual(y.min(), box.y().min());
				Assert::AreEqual(y.max(), box.y().max());
				Assert::AreEqual(z.min(), box.z().min());
				Assert::AreEqual(z.max(), box.z().max());
			}

			const glm::vec3 p1(-1.f, 2.f, 0.f);
			const glm::vec3 p2(1.f, 1.f, 2.f);

			{
				AABox box(p1, p2);
				Assert::AreEqual(p1.x, box.x().min());
				Assert::AreEqual(p2.x, box.x().max());
				Assert::AreEqual(p2.y, box.y().min());
				Assert::AreEqual(p1.y, box.y().max());
				Assert::AreEqual(p1.z, box.z().min());
				Assert::AreEqual(p2.z, box.z().max());
			}

			{
				AABox box(p2, p1);
				Assert::AreEqual(p1.x, box.x().min());
				Assert::AreEqual(p2.x, box.x().max());
				Assert::AreEqual(p2.y, box.y().min());
				Assert::AreEqual(p1.y, box.y().max());
				Assert::AreEqual(p1.z, box.z().min());
				Assert::AreEqual(p2.z, box.z().max());
			}
		}

		TEST_METHOD(TestGetInterval)
		{
			Interval x(-1.f, 1.f);
			Interval y(-2.f, -1.f);
			Interval z(0.f, 2.f);
			AABox box(x, y, z);
			Assert::IsTrue(box.getInterval(0) == x);
			Assert::IsTrue(box.getInterval(1) == y);
			Assert::IsTrue(box.getInterval(2) == z);
		}

		TEST_METHOD(TestLongestAxis)
		{
			// x
			{
				AABox box(Interval(-1.f, 5.f), Interval(-2.f, -1.f), Interval(0.f, 2.f));
				Assert::AreEqual(0, box.longestAxis());
			}

			// y
			{
				AABox box(Interval(-1.f, 1.f), Interval(-2.f, 1.f), Interval(0.f, 2.f));
				Assert::AreEqual(1, box.longestAxis());
			}
			
			// z
			{
				AABox box(Interval(-1.f, 1.f), Interval(-2.f, -1.f), Interval(-10.f, 2.f));
				Assert::AreEqual(2, box.longestAxis());
			}

			// x == y
			{
				AABox box(Interval(-1.f, 1.f), Interval(-2.f, 0.f), Interval(0.f, 1.f));
				int longestAxis = box.longestAxis();
				Assert::IsTrue(longestAxis == 0 || longestAxis == 1);
			}

			// y == z
			{
				AABox box(Interval(-1.f, 0.f), Interval(-2.f, 0.f), Interval(0.f, 2.f));
				int longestAxis = box.longestAxis();
				Assert::IsTrue(longestAxis == 1 || longestAxis == 2);
			}

			// x == z
			{
				AABox box(Interval(-5.f, 1.f), Interval(-2.f, 0.f), Interval(0.f, 6.f));
				int longestAxis = box.longestAxis();
				Assert::IsTrue(longestAxis == 0 || longestAxis == 2);
			}
		}

		TEST_METHOD(TestEquality)
		{
			testEqualityHelper(AABox(), AABox(), true);
			testEqualityHelper(AABox(), AABox({ 0.f, 0.f }, { 0.f, 0.f }, { 0.f, 0.f }), false);

			testEqualityHelper(
				AABox({ 1.f, 3.f }, { -4.f, -1.f }, { 2.f, 2.f }),
				AABox({ 1.f, 3.f }, { -4.f, -1.f }, { 2.f, 2.f }), 
				true
			);
			testEqualityHelper(
				AABox({ 1.f, 3.f }, { -4.f, -1.f }, { 2.f, 2.f }),
				AABox({ 2.f, 3.f }, { -4.f, -1.f }, { 2.f, 2.f }),
				false
			);
			testEqualityHelper(
				AABox({ 1.f, 3.f }, { -4.f, -1.f }, { 2.f, 2.f }),
				AABox({ 1.f, 3.f }, { -3.f, -1.f }, { 2.f, 2.f }),
				false
			);
			testEqualityHelper(
				AABox({ 1.f, 3.f }, { -4.f, -1.f }, { 2.f, 2.f }),
				AABox({ 1.f, 3.f }, { -4.f, -1.f }, { 2.f, 3.f }),
				false
			);
			testEqualityHelper(
				AABox({ 1.f, 3.f }, { -4.f, -1.f }, { 2.f, 2.f }),
				AABox({ -4.f, -1.f }, { 1.f, 3.f }, { 2.f, 2.f }),
				false
			);
			testEqualityHelper(
				AABox({ 1.f, 3.f }, { -4.f, -1.f }, { 2.f, 2.f }),
				AABox({ -4.f, -1.f }, { 2.f, 2.f }, { 3.f, 1.f }),
				false
			);
		}

		void testExpandBoxHelper(const AABox& box, const AABox& other, const AABox& expected) {
			// test both directions
			{
				AABox copy = box;
				copy.expand(other);
				Assert::IsTrue(copy == expected);
			}
			{
				AABox copy = other;
				copy.expand(box);
				Assert::IsTrue(copy == expected);
			}
		}

		TEST_METHOD(TestExpandBox)
		{
			// cases should mostly be covered by Interval
			testExpandBoxHelper(
				AABox(),
				AABox({ 0.f, 0.f }, { -1.f, 2.f }, { 0.f, 1.f }),
				AABox({ 0.f, 0.f }, { -1.f, 2.f }, { 0.f, 1.f })
			);
			testExpandBoxHelper(
				AABox({ 0.f, 1.f }, { -1.f, 2.f }, { 0.f, 1.f }),
				AABox({ 2.f, 3.f }, { 0.f, 2.f }, { 0.f, 1.f }),
				AABox({ 0.f, 3.f }, { -1.f, 2.f }, { 0.f, 1.f })
			);
			testExpandBoxHelper(
				AABox({ 0.f, 1.f }, { -1.f, 2.f }, { 0.f, 1.f }),
				AABox({ -1.f, 0.f }, { -1.f, 1.f }, { -5.f, 3.f }),
				AABox({ -1.f, 1.f }, { -1.f, 2.f }, { -5.f, 3.f })
			);
		}

		void testExpandPointHelper(AABox box, const glm::vec3& point, AABox expected) {
			AABox copy = box;
			copy.expand(point);
			Assert::IsTrue(copy == expected);
		}

		TEST_METHOD(TestExpandPoint)
		{
			testExpandPointHelper(
				AABox({-5.f, -5.f}, {1.f, 2.f}, {-1.f, 4.f}),
				glm::vec3(-5.f, 2.f, -1.f),
				AABox({ -5.f, -5.f }, { 1.f, 2.f }, { -1.f, 4.f })
			);
			testExpandPointHelper(
				AABox({ -5.f, -5.f }, { 1.f, 2.f }, { -1.f, 4.f }),
				glm::vec3(-5.f, 1.f, 4.f),
				AABox({ -5.f, -5.f }, { 1.f, 2.f }, { -1.f, 4.f })
			);
			testExpandPointHelper(
				AABox({ -5.f, -5.f }, { 1.f, 2.f }, { -1.f, 4.f }),
				glm::vec3(-1.f, -1.f, 6.f),
				AABox({ -5.f, -1.f }, { -1.f, 2.f }, { -1.f, 6.f })
			);
			testExpandPointHelper(
				AABox({ -5.f, -5.f }, { 1.f, 2.f }, { -1.f, 4.f }),
				glm::vec3(-6.f, 3.f, -2.f),
				AABox({ -6.f, -5.f }, { 1.f, 3.f }, { -2.f, 4.f })
			);
		}

		TEST_METHOD(TestHit)
		{
			// TODO: could probably be more principled about cases
			// visualization: https://www.desmos.com/3d/ldaxkd0f1a

			const AABox box({ -2.f, 1.f }, { -1.f, 1.f }, { -1.f, 2.f });

			// rays from inside, hitting all 6 faces + some corners
			Assert::IsTrue(box.hit(
				Ray(glm::vec3(0.f), glm::vec3(1.f, 0.f, 1.f)),
				Interval::positive
			));
			Assert::IsTrue(box.hit(
				Ray(glm::vec3(0.f), glm::vec3(-1.f, 0.f, 1.f)),
				Interval::positive
			));
			Assert::IsTrue(box.hit(
				Ray(glm::vec3(0.f), glm::vec3(0.f, 1.f, 1.f)),
				Interval::positive
			));
			Assert::IsTrue(box.hit(
				Ray(glm::vec3(0.f), glm::vec3(0.f, -1.f, 1.f)),
				Interval::positive
			));
			Assert::IsTrue(box.hit(
				Ray(glm::vec3(0.f), glm::vec3(-0.5f, 0.f, 1.f)),
				Interval::positive
			));
			Assert::IsTrue(box.hit(
				Ray(glm::vec3(0.f), glm::vec3(-0.5f, 0.f, -1.f)),
				Interval::positive
			));
			Assert::IsTrue(box.hit(
				Ray(glm::vec3(0.f), glm::vec3(1.f)),
				Interval::positive
			));
			Assert::IsTrue(box.hit(
				Ray(glm::vec3(0.f), glm::vec3(-1.f)),
				Interval::positive
			));

			// rays from outside, hit
			Assert::IsTrue(box.hit(
				Ray(glm::vec3(2.f, 1.f, 2.f), glm::vec3(-1.f)),
				Interval::positive
			));
			Assert::IsTrue(box.hit(
				Ray(glm::vec3(2.f, 3.f, 2.f), glm::vec3(-1.f)),
				Interval::positive
			));
			Assert::IsTrue(box.hit(
				Ray(glm::vec3(3.f, 0.f, 1.f), glm::vec3(-1.f, 0.f, 0.f)),
				Interval::positive
			));
			Assert::IsTrue(box.hit(
				Ray(glm::vec3(0.f, -2.f, 1.f), glm::vec3(0.f, 1.f, -0.5f)),
				Interval::positive
			));
			Assert::IsTrue(box.hit(
				Ray(glm::vec3(-1.f, 0.f, 3.f), glm::vec3(0.5f, 0.f, -2.f)),
				Interval::positive
			));

			// rays from outside, grazing hit
			Assert::IsTrue(box.hit(
				Ray(glm::vec3(-2.f, -3.f, 1.f), glm::vec3(2.99f, 2.f, -0.5f)),
				Interval::positive
			));
			Assert::IsTrue(box.hit(
				Ray(glm::vec3(-2.f, -3.f, 1.f), glm::vec3(1.f, 2.f, 0.99f)),
				Interval::positive
			));
			Assert::IsTrue(box.hit(
				Ray(glm::vec3(-2.f, -3.f, 1.f), glm::vec3(2.99f, 2.f, 0.99f)),
				Interval::positive
			));
			Assert::IsTrue(box.hit(
				Ray(glm::vec3(1.f, 0.99f, -3.f), glm::vec3(-0.01f, 0.f, 1.f)),
				Interval::positive
			));
			Assert::IsTrue(box.hit(
				Ray(glm::vec3(1.f, 0.f, 1.99f), glm::vec3(-0.01f, 1.f, 0.f)),
				Interval::positive
			));

			// rays from outside, miss
			Assert::IsFalse(box.hit(
				Ray(glm::vec3(-1.f, 0.f, 3.f), glm::vec3(0.f, 1.f, -0.5f)),
				Interval::positive
			));
			Assert::IsFalse(box.hit(
				Ray(glm::vec3(-1.f, 0.f, 2.f), glm::vec3(2.f, 0.f, 0.f)),
				Interval::positive
			));
			Assert::IsFalse(box.hit(
				Ray(glm::vec3(-1.f, -1.f, 2.f), glm::vec3(0.f, 1.f, 0.f)),
				Interval::positive
			));
			Assert::IsFalse(box.hit(
				Ray(glm::vec3(0.f, 0.f, 3.f), glm::vec3(0.f, 0.f, 1.f)),
				Interval::positive
			));
			Assert::IsFalse(box.hit(
				Ray(glm::vec3(-2.f, -3.f, 1.f), glm::vec3(4.f, 2.f, -0.5f)),
				Interval::positive
			));

			// testing t range
			const Ray rangeTestRay = Ray(glm::vec3(2.f, -1.f, 1.f), glm::vec3(-1.f, 1.f, -1.f));
			Assert::IsTrue(box.hit(rangeTestRay, Interval::positive));
			Assert::IsTrue(box.hit(rangeTestRay, Interval(0.9f, 1.5f)));
			Assert::IsTrue(box.hit(rangeTestRay, Interval(1.5f, 2.1f)));
			Assert::IsFalse(box.hit(rangeTestRay, Interval(-infinity, 0.9f)));
			Assert::IsFalse(box.hit(rangeTestRay, Interval(2.1f, infinity)));
			Assert::IsTrue(box.hit(Ray(rangeTestRay.origin(), -rangeTestRay.direction()), Interval(-infinity, 0.f)));

			// degenerate ray
			Assert::IsFalse(box.hit(Ray(), Interval::positive));

			// ray origin is on box - hit should be true if there is an intersection not at t = 0
			Assert::IsTrue(box.hit(
				Ray(glm::vec3(1.f, 0.f, 0.f), glm::vec3(-1.f, 1.f, 1.f)),
				Interval::positive
			));
			Assert::IsTrue(box.hit(
				Ray(glm::vec3(1.f, 0.f, 0.f), glm::vec3(-1.f, 0.f, 0.f)),
				Interval::positive
			));
			Assert::IsTrue(box.hit(
				Ray(glm::vec3(1.f, 0.f, 0.f), glm::vec3(-1.f, 0.f, 0.f)),
				Interval::positive
			));
			Assert::IsFalse(box.hit(
				Ray(glm::vec3(1.f, 0.f, 0.f), glm::vec3(1.f, 0.f, 0.f)),
				Interval::positive
			));
			Assert::IsFalse(box.hit(
				Ray(glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f)),
				Interval::positive
			));
			Assert::IsFalse(box.hit(
				Ray(glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 1.f)),
				Interval::positive
			));
		}
	};
}