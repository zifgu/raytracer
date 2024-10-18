#include "pch.h"
#include "CppUnitTest.h"

#include "test_common.h"
#include "test_hittable.h"
#include "../src/sphere.h"
#include "../src/lambertian.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	TEST_CLASS(TestSphere)
	{
		const std::shared_ptr<Material> dummyMaterial = std::make_shared<Lambertian>(glm::vec3(1.f));
	public:
		TEST_METHOD(TestConstructor)
		{
			const glm::vec3 center(1.f, 2.f, 3.f);
			const float radius = 1.5f;
			const Sphere sphere(center, radius, dummyMaterial);
			Assert::AreEqual(center, sphere.center());
			Assert::AreEqual(radius, sphere.radius());
			assertSharedPtrEqual(dummyMaterial, sphere.material());
		}

		TEST_METHOD(TestBoundingBox)
		{
			// unit sphere
			{
				const Sphere sphere(glm::vec3(0.f), 1.f, dummyMaterial);
				Assert::AreEqual(AABox({ -1.f, 1.f }, { -1.f, 1.f }, { -1.f, 1.f }), sphere.boundingBox());
			}

			// translated and scaled sphere
			{
				const Sphere sphere(glm::vec3(1.f, 2.f, 3.f), 1.5f, dummyMaterial);
				Assert::AreEqual(AABox({ -0.5f, 2.5f }, { 0.5f, 3.5f }, { 1.5f, 4.5f }), sphere.boundingBox());
			}

			// zero radius sphere
			{
				const Sphere sphere(glm::vec3(-1.f), 0.f, dummyMaterial);
				Assert::AreEqual(AABox({ -1.f, -1.f }, { -1.f, -1.f }, { -1.f, -1.f }), sphere.boundingBox());
			}
		}

		TEST_METHOD(TestSphereUV)
		{
			const float tolerance = 1e-4f;
			
			// basic points
			assertFuzzyEqual(glm::vec2(0.f, 0.5f), Sphere::getSphereUV(glm::vec3(-1.f, 0.f, 0.f)), tolerance);
			assertFuzzyEqual(glm::vec2(0.25f, 0.5f), Sphere::getSphereUV(glm::vec3(0.f, 0.f, 1.f)), tolerance);
			assertFuzzyEqual(glm::vec2(0.5f, 0.5f), Sphere::getSphereUV(glm::vec3(1.f, 0.f, 0.f)), tolerance);
			assertFuzzyEqual(glm::vec2(0.75f, 0.5f), Sphere::getSphereUV(glm::vec3(0.f, 0.f, -1.f)), tolerance);
			{
				glm::vec2 uv = Sphere::getSphereUV(glm::vec3(0.f, -1.f, 0.f));
				Assert::IsTrue(0.f <= uv.x && uv.x <= 1.f);	// leave u coordinate undefined at poles
				Assert::AreEqual(0.f, uv.y);
			}
			{
				glm::vec2 uv = Sphere::getSphereUV(glm::vec3(0.f, 1.f, 0.f));
				Assert::IsTrue(0.f <= uv.x && uv.x <= 1.f);	// leave u coordinate undefined at poles
				Assert::AreEqual(1.f, uv.y);
			}

			// points in between
			assertFuzzyEqual(glm::vec2(0.1667f, 0.75f), Sphere::getSphereUV(glm::vec3(-0.5f, 0.7071f, 0.866f)), tolerance);
			assertFuzzyEqual(glm::vec2(0.375f, 0.3333f), Sphere::getSphereUV(glm::vec3(0.7071f, -0.5f, 0.7071f)), tolerance);
			assertFuzzyEqual(glm::vec2(0.6762f, 0.8333f), Sphere::getSphereUV(glm::vec3(0.4472f, 0.866f, -0.8944f)), tolerance);
			assertFuzzyEqual(glm::vec2(0.9167f, 0.3524f), Sphere::getSphereUV(glm::vec3(-0.866f, -0.4472f, -0.5f)), tolerance);
		}

		// sets up ray with direction rayDir that intersects (0, 0, 1) on the unit sphere at t = 1
		// assumes this is the first intersection in the positive half of the ray
		void getHitOnUnitSphere(const glm::vec3& rayDir, Ray& outRay, Hittable::HitRecord& outHit) const {
			const glm::vec3 point(0.f, 0.f, 1.f);
			const glm::vec3 rayOrigin = point - rayDir;

			const glm::vec3 expectOutwardNormal = glm::normalize(point);
			outHit.material = dummyMaterial;
			outHit.point = point;

			if (glm::length(rayOrigin) < 1.f) {
				// ray is inside
				outHit.normal = -expectOutwardNormal;
				outHit.frontFace = false;
			}
			else {
				// ray is outside
				outHit.normal = expectOutwardNormal;
				outHit.frontFace = true;
			}

			outRay = Ray(rayOrigin, rayDir);
		}

		// test ray hits against given sphere by transforming a number of cases defined against the unit sphere
		void testHitHelper(const Sphere& sphere, const glm::mat3& rotation, float tolerance) const {
			// pairs [rayDir, safeTDist] defined against the unit sphere
			const std::pair<glm::vec3, float> testRays[6] = {
				// from inside
				{ glm::vec3(0.f, 0.f, 1.f), 0.5f },
				{ glm::vec3(0.5f, 0.5f, 0.5f), 0.5f },

				// from outside
				{ glm::vec3(0.3f, 0.5f, -0.3f), 0.5f },
				{ glm::vec3(-1.f, 2.f, -0.9f), 0.25f },
				{ glm::vec3(-1.f, -0.5f, -4.f), 0.25f },
				{ glm::vec3(1.5f, -0.5f, -0.5f), 0.25f },
			};

			for (const auto& pair : testRays) {
				// set up test case on unit sphere
				Hittable::HitRecord expectHit; Ray ray;
				getHitOnUnitSphere(pair.first, ray, expectHit);

				// transform ray and hit data
				expectHit.normal = rotation * expectHit.normal;
				expectHit.point = sphere.center() + rotation * sphere.radius() * expectHit.point;
				expectHit.uv = Sphere::getSphereUV((expectHit.frontFace ? 1.f : -1.f) * expectHit.normal);

				Ray transformedRay = Ray(
					sphere.center() + rotation * sphere.radius() * ray.origin(),
					rotation * sphere.radius() * ray.direction()
				);

				testHitTScalesAndTRange(sphere, transformedRay, expectHit, 1.f, pair.second, tolerance);
			}
		}

		TEST_METHOD(TestHit)
		{
			// unit sphere
			testHitHelper(Sphere(glm::vec3(0.f), 1.f, dummyMaterial), glm::mat3(1.f), 1e-4f);

			// various translated, rotated, and scaled configurations
			testHitHelper(
				Sphere(glm::vec3(-30.f, 30.f, -2.f), 2.f, dummyMaterial),
				glm::eulerAngleXYZ(glm::radians(30.f), glm::radians(-10.f), glm::radians(100.f)),
				1e-4f
			);
			testHitHelper(
				Sphere(glm::vec3(2.f, -1.f, -200.f), 5.f, dummyMaterial),
				glm::eulerAngleXYZ(glm::radians(-60.f), glm::radians(90.f), glm::radians(50.f)),
				1e-4f
			);
			testHitHelper(
				Sphere(glm::vec3(100.f, -50.f, 2.f), 200.f, dummyMaterial),
				glm::eulerAngleXYZ(glm::radians(135.f), glm::radians(30.f), glm::radians(45.f)),
				2e-3f
			);
			testHitHelper(
				Sphere(glm::vec3(10.f, -10.f, 80.f), 500.f, dummyMaterial),
				glm::eulerAngleXYZ(glm::radians(180.f), glm::radians(-135.f), glm::radians(-30.f)),
				2e-3f
			);

			// hits with exactly 1 intersection
			{
				Hittable::HitRecord expectHit;
				glm::vec3 unitDir = glm::vec3(1.f, 0.f, 0.f);
				expectHit.point = unitDir;
				expectHit.normal = unitDir;
				expectHit.frontFace = true;
				expectHit.t = 1.f;
				expectHit.uv = Sphere::getSphereUV(unitDir);
				expectHit.material = dummyMaterial;

				assertHit(
					Sphere(glm::vec3(0.f), 1.f, dummyMaterial),
					Ray(glm::vec3(1.f), glm::vec3(0.f, -1.f, -1.f)),
					Interval::positive,
					expectHit,
					1e-4f
				);
			}
			{
				Hittable::HitRecord expectHit;
				glm::vec3 expectedOutwardNormal = glm::normalize(glm::vec3(3.f, 4.f, 0.f));
				expectHit.point = glm::vec3(2.f, 3.f, 0.f);
				expectHit.normal = expectedOutwardNormal;
				expectHit.frontFace = true;
				expectHit.t = 0.5f;
				expectHit.uv = Sphere::getSphereUV(expectedOutwardNormal);
				expectHit.material = dummyMaterial;

				assertHit(
					Sphere(glm::vec3(-1.f, -1.f, 0.f), 5.f, dummyMaterial),
					Ray(glm::vec3(-2.f, 6.f, 0.f), glm::vec3(8.f, -6.f, 0.f)),
					Interval::positive,
					expectHit,
					1e-4f
				);
			}

			// ray origin exactly on sphere
			assertMiss(
				Sphere(glm::vec3(0.f), 1.f, dummyMaterial),
				Ray(glm::vec3(1.f, 0.f, 0.f), glm::vec3(1.f, 1.f, 1.f)),
				Interval::positive
			);
			assertMiss(
				Sphere(glm::vec3(1.f, 0.f, 0.f), 1.f, dummyMaterial),
				Ray(glm::vec3(2.f, 0.f, 0.f), glm::vec3(0.f, 2.f, 0.f)),
				Interval::positive
			);
			assertMiss(
				Sphere(glm::vec3(-1.f), 2.f, dummyMaterial),
				Ray(glm::vec3(-1.f, -1.f, 1.f), glm::vec3(0.f, 0.f, 2.f)),
				Interval::positive
			);

			// ray origin is exactly on sphere, but a different intersection exists
			{
				Hittable::HitRecord expectHit;
				glm::vec3 unitDir = glm::vec3(1.f / 3.f, 2.f / 3.f, 2.f / 3.f);
				expectHit.point = unitDir;
				expectHit.normal = -unitDir;
				expectHit.frontFace = false;
				expectHit.t = 2.f / 3.f;
				expectHit.uv = Sphere::getSphereUV(unitDir);
				expectHit.material = dummyMaterial;

				assertHit(
					Sphere(glm::vec3(0.f), 1.f, dummyMaterial),
					Ray(glm::vec3(1.f, 0.f, 0.f), glm::vec3(-1.f, 1.f, 1.f)),
					Interval::positive,
					expectHit,
					1e-4f
				);
			}
			{
				Hittable::HitRecord expectHit;
				expectHit.point = glm::vec3(-1.f, -1.f, -3.f);
				expectHit.normal = glm::vec3(0.f, 0.f, 1.f);
				expectHit.frontFace = false;
				expectHit.t = 2.f;
				expectHit.uv = Sphere::getSphereUV(-expectHit.normal);
				expectHit.material = dummyMaterial;

				assertHit(
					Sphere(glm::vec3(-1.f), 2.f, dummyMaterial),
					Ray(glm::vec3(-1.f, -1.f, 1.f), glm::vec3(0.f, 0.f, -2.f)),
					Interval::positive,
					expectHit,
					1e-4f
				);
			}

			// hits with degenerate rays
			assertMiss(Sphere(glm::vec3(-10.f), 30.f, dummyMaterial), Ray(), Interval::positive);
			assertMiss(
				Sphere(glm::vec3(1.f, 0.f, 0.f), 1.f, dummyMaterial),
				Ray(glm::vec3(1.f, 1.f, 0.f), glm::vec3(0.f)),
				Interval::positive
			);
		}
	};
}