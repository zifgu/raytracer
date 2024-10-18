#include "pch.h"
#include "CppUnitTest.h"

#include "test_common.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	TEST_CLASS(TestRay)
	{
	public:
		TEST_METHOD(TestConstructor)
		{
			// default
			{
				Ray ray;
				Assert::AreEqual(glm::vec3(0.f), ray.origin());
				Assert::AreEqual(glm::vec3(0.f), ray.direction());
			}

			// with args
			{
				glm::vec3 origin(1.f);
				glm::vec3 direction(-1.f, 2.f, -1.f);
				Ray ray(origin, direction);
				Assert::AreEqual(origin, ray.origin());
				Assert::AreEqual(direction, ray.direction());
			}
		}

		TEST_METHOD(TestEquality)
		{
			testEqualityHelper(Ray(), Ray(), true);
			testEqualityHelper(
				Ray({ -1.f, 2.f, 0.f }, { 0.f, 1.f, 2.f }),
				Ray({ -1.f, 2.f, 0.f }, { 0.f, 1.f, 2.f }),
				true
			);
			testEqualityHelper(
				Ray({ -1.f, 2.f, 0.f }, { 0.f, 1.f, 2.f }),
				Ray({ -1.f, 2.f, 0.f }, { 0.f, 2.f, 4.f }),
				false
			);
			testEqualityHelper(
				Ray({ -1.f, 2.f, 0.f }, { 0.f, 1.f, 2.f }),
				Ray({ 0.f, 1.f, 2.f } , { -1.f, 2.f, 0.f }),
				false
			);
			testEqualityHelper(
				Ray({ -1.f, 1.f, 0.f }, { 0.f, 1.f, 2.f }),
				Ray({ -1.f, 2.f, 0.f }, { 0.f, 1.f, 2.f }),
				false
			);
			testEqualityHelper(
				Ray({ -1.f, 1.f, 0.f }, { 0.f, 1.f, 2.f }),
				Ray({ -1.f, 2.f, 0.f }, { 1.f, 1.f, 2.f }),
				false
			);
			testEqualityHelper(
				Ray({ -1.f, 1.f, 0.f }, { 0.f, 1.f, 2.f }),
				Ray({ -3.f, 2.f, 0.f }, { 1.f, 1.f, 6.f }),
				false
			);
		}

		TEST_METHOD(TestAt)
		{
			// degenerate rays
			{
				Ray ray;
				Assert::AreEqual(glm::vec3(0.f), ray.at(0.f));
				Assert::AreEqual(glm::vec3(0.f), ray.at(1.f));
				Assert::AreEqual(glm::vec3(0.f), ray.at(-1.f));
			}
			{
				Ray ray(glm::vec3(1.f), glm::vec3(0.f));
				Assert::AreEqual(glm::vec3(1.f), ray.at(0.f));
				Assert::AreEqual(glm::vec3(1.f), ray.at(1.f));
				Assert::AreEqual(glm::vec3(1.f), ray.at(-1.f));
			}

			// regular rays
			{
				Ray ray(glm::vec3(-1.f), glm::vec3(1.f, 2.f, -2.f));
				Assert::AreEqual(glm::vec3(-1.f), ray.at(0.f));
				Assert::AreEqual(glm::vec3(-0.5f, 0.f, -2.f), ray.at(0.5f));
				Assert::AreEqual(glm::vec3(1.f, 3.f, -5.f), ray.at(2.f));
				Assert::AreEqual(glm::vec3(-3.f, -5.f, 3.f), ray.at(-2.f));
			}
			{
				Ray ray(glm::vec3(1.f, 2.f, 3.f), glm::vec3(2.f, 0.f, 0.f));
				Assert::AreEqual(glm::vec3(1.f, 2.f, 3.f), ray.at(0.f));
				Assert::AreEqual(glm::vec3(3.f, 2.f, 3.f), ray.at(1.f));
				Assert::AreEqual(glm::vec3(-1.f, 2.f, 3.f), ray.at(-1.f));
			}
		}
	};
}