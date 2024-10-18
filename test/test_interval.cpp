#include "pch.h"
#include "CppUnitTest.h"

#include "test_common.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	TEST_CLASS(TestInterval)
	{
	public:
		TEST_METHOD(TestConstructor)
		{
			// default
			{
				Interval interval;
				Assert::AreEqual(infinity, interval.min());
				Assert::AreEqual(-infinity, interval.max());
			}

			// with args
			{
				float min = -2.f;
				float max = 1.f;
				Interval interval(min, max);
				Assert::AreEqual(min, interval.min());
				Assert::AreEqual(max, interval.max());
			}
		}

		TEST_METHOD(TestSize)
		{
			// valid interval
			{
				Interval interval(-2.f, 1.f);
				Assert::AreEqual(3.f, interval.size());
			}

			// reversed interval
			{
				Interval interval(2.f, -1.f);
				Assert::AreEqual(-3.f, interval.size());
			}

			// degenerate interval
			{
				Interval interval(1.f, 1.f);
				Assert::AreEqual(0.f, interval.size());
			}
		}

		TEST_METHOD(TestContains)
		{
			float min = -5.f;
			float max = 3.f;
			float mid = -1.f;

			// valid interval
			{
				Interval interval(min, max);
				Assert::IsTrue(interval.contains(min));
				Assert::IsTrue(interval.contains(max));
				Assert::IsTrue(interval.contains(mid));
				Assert::IsFalse(interval.contains(min - 0.1f));
				Assert::IsFalse(interval.contains(max + 0.1f));
			}

			// reversed interval
			{
				Interval interval(max, min);
				Assert::IsFalse(interval.contains(min));
				Assert::IsFalse(interval.contains(max));
				Assert::IsFalse(interval.contains(mid));
				Assert::IsFalse(interval.contains(min - 0.1f));
				Assert::IsFalse(interval.contains(max + 0.1f));
			}

			// degenerate interval
			{
				Interval interval(min, min);
				Assert::IsTrue(interval.contains(min));
				Assert::IsFalse(interval.contains(min - 0.1f));
				Assert::IsFalse(interval.contains(min + 0.1f));
			}
		}

		TEST_METHOD(TestSurrounds)
		{
			float min = -5.f;
			float max = 3.f;
			float mid = -1.f;

			// valid interval
			{
				Interval interval(min, max);
				Assert::IsFalse(interval.surrounds(min));
				Assert::IsFalse(interval.surrounds(max));
				Assert::IsTrue(interval.surrounds(mid));
				Assert::IsFalse(interval.surrounds(min - 0.1f));
				Assert::IsFalse(interval.surrounds(max + 0.1f));
			}

			// reversed interval
			{
				Interval interval(max, min);
				Assert::IsFalse(interval.surrounds(min));
				Assert::IsFalse(interval.surrounds(max));
				Assert::IsFalse(interval.surrounds(mid));
				Assert::IsFalse(interval.surrounds(min - 0.1f));
				Assert::IsFalse(interval.surrounds(max + 0.1f));
			}

			// degenerate interval
			{
				Interval interval(min, min);
				Assert::IsFalse(interval.surrounds(min));
				Assert::IsFalse(interval.surrounds(min - 0.1f));
				Assert::IsFalse(interval.surrounds(min + 0.1f));
			}
		}

		TEST_METHOD(TestClamp)
		{
			float min = 0.f;
			float max = 1.f;
			float mid = 0.5f * (min + max);

			// valid interval
			{
				Interval interval(min, max);
				Assert::AreEqual(min, interval.clamp(min - 0.5f));
				Assert::AreEqual(min, interval.clamp(min));
				Assert::AreEqual(mid, interval.clamp(mid));
				Assert::AreEqual(max, interval.clamp(max));
				Assert::AreEqual(max, interval.clamp(max + 0.5f));
			}

			// leaving results undefined if interval is reversed

			// degenerate interval
			{
				Interval interval(min, min);
				Assert::AreEqual(min, interval.clamp(min - 0.5f));
				Assert::AreEqual(min, interval.clamp(min));
				Assert::AreEqual(min, interval.clamp(min + 0.5f));
			}
		}

		void testEqualityHelper(Interval a, Interval b, bool expectEqual)
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

		TEST_METHOD(TestEquality)
		{
			testEqualityHelper(Interval(), Interval(), true);
			testEqualityHelper(Interval(), Interval(0.f, 0.f), false);
			testEqualityHelper(Interval(1.f, 2.f), Interval(1.f, 2.f), true);
			testEqualityHelper(Interval(1.f, 2.f), Interval(1.f, 1.f), false);
			testEqualityHelper(Interval(1.f, 2.f), Interval(2.f, 2.f), false);
			testEqualityHelper(Interval(1.f, 2.f), Interval(2.f, 1.f), false);
		}

		void testExpandHelper(const Interval& interval, const Interval& other, const Interval& expected) {
			// test both directions
			{
				Interval copy = interval;
				copy.expand(other);
				Assert::IsTrue(copy == expected);
			}
			{
				Interval copy = other;
				copy.expand(interval);
				Assert::IsTrue(copy == expected);
			}
		}

		TEST_METHOD(TestExpandInterval)
		{
			// from default empty interval
			testExpandHelper(Interval(), Interval(-2.f, 10.f), Interval(-2.f, 10.f));
			testExpandHelper(Interval(), Interval(10.f, -2.f), Interval(10.f, -2.f));
			testExpandHelper(Interval(), Interval(-2.f, -2.f), Interval(-2.f, -2.f));

			// valid intervals, non-overlapping
			testExpandHelper(Interval(-1.f, 1.f), Interval(-3.f, -2.f), Interval(-3.f, 1.f));

			// valid intervals, overlapping
			testExpandHelper(Interval(-1.f, 1.f), Interval(0.f, 1.f), Interval(-1.f, 1.f));
			testExpandHelper(Interval(-1.f, 1.f), Interval(-1.f, 0.f), Interval(-1.f, 1.f));
			testExpandHelper(Interval(-1.f, 1.f), Interval(0.f, 2.f), Interval(-1.f, 2.f));
			testExpandHelper(Interval(-1.f, 1.f), Interval(-2.f, 2.f), Interval(-2.f, 2.f));
			testExpandHelper(Interval(-1.f, 1.f), Interval(-2.f, -1.f), Interval(-2.f, 1.f));
			testExpandHelper(Interval(-1.f, 1.f), Interval(1.f, 2.f), Interval(-1.f, 2.f));

			// valid and degenerate intervals
			testExpandHelper(Interval(-1.f, 1.f), Interval(0.f, 0.f), Interval(-1.f, 1.f));
			testExpandHelper(Interval(-1.f, 1.f), Interval(-1.f, -1.f), Interval(-1.f, 1.f));
			testExpandHelper(Interval(-1.f, 1.f), Interval(1.f, 1.f), Interval(-1.f, 1.f));
			testExpandHelper(Interval(-1.f, 1.f), Interval(-2.f, -2.f), Interval(-2.f, 1.f));
			testExpandHelper(Interval(-1.f, 1.f), Interval(2.f, 2.f), Interval(-1.f, 2.f));

			// valid and reversed intervals
			testExpandHelper(Interval(-1.f, 1.f), Interval(10.f, -5.f), Interval(-1.f, 1.f));
			testExpandHelper(Interval(-1.f, 1.f), Interval(0.f, -5.f), Interval(-1.f, 1.f));
			testExpandHelper(Interval(-1.f, 1.f), Interval(5.f, 0.f), Interval(-1.f, 1.f));
			testExpandHelper(Interval(-1.f, 1.f), Interval(1.f, -1.f), Interval(-1.f, 1.f));

			// leaving remaining behaviour undefined for reversed intervals because it's... pretty weird
		}

		TEST_METHOD(TestExpandValue)
		{
			{
				Interval interval(-1.f, 1.f);
				interval.expand(0.f);
				Assert::IsTrue(interval == Interval(-1.f, 1.f));
			}
			{
				Interval interval(-1.f, 1.f);
				interval.expand(-1.f);
				Assert::IsTrue(interval == Interval(-1.f, 1.f));
			}
			{
				Interval interval(-1.f, 1.f);
				interval.expand(1.f);
				Assert::IsTrue(interval == Interval(-1.f, 1.f));
			}
			{
				Interval interval(-1.f, 1.f);
				interval.expand(-2.f);
				Assert::IsTrue(interval == Interval(-2.f, 1.f));
			}
			{
				Interval interval(-1.f, 1.f);
				interval.expand(2.f);
				Assert::IsTrue(interval == Interval(-1.f, 2.f));
			}
		}

		TEST_METHOD(TestPad)
		{
			// pad 0
			{
				Interval interval(-1.f, 1.f);
				interval.pad(0.f);
				Assert::IsTrue(interval == Interval(-1.f, 1.f));
			}

			// pad positive
			{
				Interval interval(-1.f, 1.f);
				float delta = 1e-3f;
				interval.pad(2.f * delta);
				Assert::IsTrue(interval == Interval(-1.f - delta, 1.f + delta));
			}

			// pad negative
			{
				Interval interval(-1.f, 1.f);
				float delta = 1e-3f;
				interval.pad(-2.f * delta);
				Assert::IsTrue(interval == Interval(-1.f + delta, 1.f - delta));
			}
		}
	};
}
