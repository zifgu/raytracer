#include "pch.h"
#include "CppUnitTest.h"

#include "test_common.h"
#include "../src/camera.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	TEST_CLASS(TestCamera)
	{
	public:
		TEST_METHOD(TestFrameConstructor)
		{
			// default up vector
			{
				Camera::Frame frame(
					glm::vec3(2.f, 3.f, 1.f),
					glm::vec3(0.f, 1.f, -1.f)
				);
				Assert::AreEqual(frame.pos(), glm::vec3(2.f, 3.f, 1.f));

				// Frame axes should be unit vectors
				const float tolerance = 1e-4f;
				assertFuzzyEqual(glm::normalize(glm::vec3(1.f, 0.f, -1.f)), frame.x(), tolerance);
				assertFuzzyEqual(glm::normalize(glm::vec3(-1.f, 2.f, -1.f)), frame.y(), tolerance);
				assertFuzzyEqual(glm::normalize(glm::vec3(1.f, 1.f, 1.f)), frame.z(), tolerance);
			}

			// custom up vector
			{
				Camera::Frame frame(
					glm::vec3(0.f, 1.f, -1.f),
					glm::vec3(2.f, 3.f, 1.f),
					glm::vec3(0.f, -1.f, -1.f)
				);
				Assert::AreEqual(frame.pos(), glm::vec3(0.f, 1.f, -1.f));

				// Frame axes should be unit vectors
				const float tolerance = 1e-4f;
				assertFuzzyEqual(glm::normalize(glm::vec3(0.f, 1.f, -1.f)), frame.x(), tolerance);
				assertFuzzyEqual(glm::normalize(glm::vec3(2.f, -1.f, -1.f)), frame.y(), tolerance);
				assertFuzzyEqual(glm::normalize(glm::vec3(-1.f, -1.f, -1.f)), frame.z(), tolerance);
			}
		}

		TEST_METHOD(TestProjectionConstructor)
		{
			Camera::Projection proj(glm::ivec2(90, 160), 90.f, 1.f);
			Assert::AreEqual(glm::vec2(90.f, 160.f), proj.imageSize());
			Assert::AreEqual(90.f, proj.fovDegreesVertical());
			Assert::AreEqual(1.f, proj.focalLength());
			Assert::AreEqual(0.5625f, proj.aspectRatio());
		}

		typedef void(*GetRayTestCase)(const glm::vec3&, const glm::mat3&, bool);

		// exact equality of origin, fuzzy equality of rotated direction
		static void testRayEqualityHelper(const Ray& ray, const glm::vec3& expectOrigin, const glm::vec3& expectDirection, const glm::mat3& inverseRotation, const float halfPixelDelta, bool jitter) {
			Assert::AreEqual(expectOrigin, ray.origin());

			// rotate ray back to "canonical" image plane (which is assumed parallel to xy-plane), compare against expected
			const float epsilon = 1e-4f;
			const float jitterTolerance = jitter ? halfPixelDelta : 0.f;
			const glm::vec3& canonicalDir = inverseRotation * ray.direction();
			Assert::AreEqual(expectDirection.x, canonicalDir.x, jitterTolerance + epsilon, L"x");
			Assert::AreEqual(expectDirection.y, canonicalDir.y, jitterTolerance + epsilon, L"y");
			Assert::AreEqual(expectDirection.z, canonicalDir.z, epsilon, L"z");
		}

		// fov = 90deg, focal dist = 1, portrait aspect ratio
		static void testGetRaySetup1(const glm::vec3& origin, const glm::mat3& rotation, bool jitter) {
			const glm::vec3 lookDir(0.f, 0.f, -1.f);
			const glm::vec3 up(0.f, 1.f, 0.f);
			const float focalDist = 1.f;
			const Camera camera(
				Camera::Frame(origin, origin + rotation * lookDir, rotation * up),
				Camera::Projection(glm::ivec2(4, 8), 90.f, focalDist)
			);
			const float halfPixelDelta = 0.125f;
			const glm::mat3 inverseRotation = glm::inverse(rotation);

			testRayEqualityHelper(
				camera.getRay(0, 0, jitter),
				origin,
				glm::vec3(-0.5f + halfPixelDelta, 1.f - halfPixelDelta, -focalDist),
				inverseRotation, halfPixelDelta, jitter
			);
			testRayEqualityHelper(
				camera.getRay(3, 0, jitter),
				origin,
				glm::vec3(0.5f - halfPixelDelta, 1.f - halfPixelDelta, -focalDist),
				inverseRotation, halfPixelDelta, jitter
			);
			testRayEqualityHelper(
				camera.getRay(0, 7, jitter),
				origin,
				glm::vec3(-0.5f + halfPixelDelta, -1.f + halfPixelDelta, -focalDist),
				inverseRotation, halfPixelDelta, jitter
			);
			testRayEqualityHelper(
				camera.getRay(3, 7, jitter),
				origin,
				glm::vec3(0.5f - halfPixelDelta, -1.f + halfPixelDelta, -focalDist),
				inverseRotation, halfPixelDelta, jitter
			);
			testRayEqualityHelper(
				camera.getRay(2, 3, jitter),
				origin,
				glm::vec3(0.25f - halfPixelDelta, 0.25f - halfPixelDelta, -focalDist),
				inverseRotation, halfPixelDelta, jitter
			);
		}

		// fov = 60deg, focal dist > 1, landscape aspect ratio
		static void testGetRaySetup2(const glm::vec3& origin, const glm::mat3& rotation, bool jitter) {
			const glm::vec3 lookDir(0.f, 0.f, 1.f);
			const glm::vec3 up(0.f, 1.f, 0.f);
			const float focalDist = 2.f * glm::sqrt(3.f);
			const Camera camera(
				Camera::Frame(origin, origin + rotation * lookDir, rotation * up),
				Camera::Projection(glm::ivec2(16, 8), 60.f, focalDist)
			);
			const float halfPixelDelta = 0.25f;
			const glm::mat3 inverseRotation = glm::inverse(rotation);

			testRayEqualityHelper(
				camera.getRay(0, 0, jitter),
				origin,
				glm::vec3(4.f - halfPixelDelta, 2.f - halfPixelDelta, focalDist),
				inverseRotation, halfPixelDelta, jitter
			);
			testRayEqualityHelper(
				camera.getRay(15, 0, jitter),
				origin,
				glm::vec3(-4.f + halfPixelDelta, 2.f - halfPixelDelta, focalDist),
				inverseRotation, halfPixelDelta, jitter
			);
			testRayEqualityHelper(
				camera.getRay(0, 7, jitter),
				origin,
				glm::vec3(4.f - halfPixelDelta, -2.f + halfPixelDelta, focalDist),
				inverseRotation, halfPixelDelta, jitter
			);
			testRayEqualityHelper(
				camera.getRay(15, 7, jitter),
				origin,
				glm::vec3(-4.f + halfPixelDelta, -2.f + halfPixelDelta, focalDist),
				inverseRotation, halfPixelDelta, jitter
			);
			testRayEqualityHelper(
				camera.getRay(6, 3, jitter),
				origin,
				glm::vec3(1.f - halfPixelDelta, 0.5f - halfPixelDelta, focalDist),
				inverseRotation, halfPixelDelta, jitter
			);
		}

		// various translations and rotations to run for each setup
		const std::pair<glm::vec3, glm::mat3> getRayTestCases[11] = {
			{ glm::vec3(0.f), glm::mat3(1.f) },	// simple case
			{ glm::vec3(1.f, -1.f, 0.f), glm::eulerAngleX(glm::radians(30.f)) },
			{ glm::vec3(0.f, -2.f, 1.f), glm::eulerAngleX(glm::radians(-80.f)) },
			{ glm::vec3(1.f, 0.f, 0.f), glm::eulerAngleY(glm::radians(120.f)) },
			{ glm::vec3(1.f, 3.f, 2.f), glm::eulerAngleY(glm::radians(-20.f)) },
			{ glm::vec3(10.f, -1.f, 1.f), glm::eulerAngleZ(glm::radians(50.f)) },
			{ glm::vec3(1.f, 50.f, 1.f), glm::eulerAngleZ(glm::radians(-150.f)) },
			{ glm::vec3(6.f, 2.f, -3.f), glm::eulerAngleXY(glm::radians(-20.f), glm::radians(80.f)) },
			{ glm::vec3(-1.f, 4.f, -1.f), glm::eulerAngleYZ(glm::radians(100.f), glm::radians(30.f)) },
			{ glm::vec3(0.1f, 0.01f, 0.5f), glm::eulerAngleXZ(glm::radians(100.f), glm::radians(90.f)) },
			{ glm::vec3(0.01f, 60.f, -10.f), glm::eulerAngleXYZ(glm::radians(-10.f), glm::radians(90.f), glm::radians(135.f)) },
		};

		TEST_METHOD(TestGetRay)
		{
			GetRayTestCase cases[2] = {
				&testGetRaySetup1,
				&testGetRaySetup2,
			};
			for (const auto& caseFn : cases) {
				for (const auto& testCase : getRayTestCases) {
					caseFn(testCase.first, testCase.second, false);
				}
			}
		}

		TEST_METHOD(TestGetRayJittered)
		{
			int randomRepeats = 100;
			GetRayTestCase cases[2] = {
				&testGetRaySetup1,
				&testGetRaySetup2,
			};
			for (const auto& caseFn : cases) {
				for (const auto& testCase : getRayTestCases) {
					for (int i = 0; i < randomRepeats; ++i) {
						caseFn(testCase.first, testCase.second, true);
					}
				}
			}
		}
	};
}