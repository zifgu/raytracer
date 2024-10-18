#include "pch.h"
#include "CppUnitTest.h"

#include <array>

#include "test_common.h"
#include "test_hittable.h"
#include "../src/quad.h"
#include "../src/lambertian.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	TEST_CLASS(TestQuad)
	{
		const std::shared_ptr<Material> dummyMaterial = std::make_shared<Lambertian>(glm::vec3(1.f));
	public:
		TEST_METHOD(TestConstructor)
		{
			const glm::vec3 corner(-1.f);
			const glm::vec3 side1(1.f, 0.f, 0.f);
			const glm::vec3 side2(0.f, 2.0f, 1.f);
			Quad quad(corner, side1, side2, dummyMaterial);
			Assert::AreEqual(corner, quad.corner());
			Assert::AreEqual(side1, quad.side1());
			Assert::AreEqual(side2, quad.side2());
			assertSharedPtrEqual(dummyMaterial, quad.material());
		}

		void testPlaneValuesHelper(const glm::vec3& corner, const glm::vec3& side1, const glm::vec3& side2, const glm::vec3& expectNormal, float expectD, float tolerance = 1e-4f) {
			{
				Quad quad(corner, side1, side2, dummyMaterial);
				assertFuzzyEqual(expectNormal, quad.planeNormal(), tolerance);
				Assert::AreEqual(expectD, quad.planeD(), tolerance);
			}

			// inverted normal
			{
				Quad quad(corner, side2, side1, dummyMaterial);
				assertFuzzyEqual(-expectNormal, quad.planeNormal(), tolerance);
				Assert::AreEqual(-expectD, quad.planeD(), tolerance);
			}
		}

		TEST_METHOD(TestPlaneValues)
		{
			// axis-aligned quads
			testPlaneValuesHelper(
				glm::vec3(3.f),
				glm::vec3(1.f, 0.f, 0.f),
				glm::vec3(0.f, 2.f, 0.f),
				glm::vec3(0.f, 0.f, 1.f),
				3.f
			);
			testPlaneValuesHelper(
				glm::vec3(-1.f, 200.f, 0.f),
				glm::vec3(2.f, 0.f, 0.f),
				glm::vec3(0.f, 0.f, 2.f),
				glm::vec3(0.f, -1.f, 0.f),
				-200.f
			);
			testPlaneValuesHelper(
				glm::vec3(10.f, 10.f, -10.f),
				glm::vec3(0.f, 10.f, 0.f),
				glm::vec3(0.f, 0.f, -2.f),
				glm::vec3(-1.f, 0.f, 0.f),
				-10.f
			);

			// various other quads
			testPlaneValuesHelper(
				glm::vec3(0.f),
				glm::vec3(2.f, 1.f, 2.f),
				glm::vec3(1.f, 5.f, -1.f),
				glm::normalize(glm::vec3(-11.f, 4.f, 9.f)),
				0.f
			);
			testPlaneValuesHelper(
				glm::vec3(0.f, 4.f, 2.f),
				glm::vec3(3.f, -1.f, -1.f),
				glm::vec3(0.5f, 2.f, -2.f),
				glm::normalize(glm::vec3(4.f, 5.5f, 6.5f)),
				3.720458f
			);
			testPlaneValuesHelper(
				glm::vec3(-10.f, -20.f, -30.f),
				glm::vec3(-1.f, 10.f, 10.f),
				glm::vec3(100.f, -8.f, -3.f),
				glm::normalize(glm::vec3(50.f, 997.f, -992.f)),
				6.622477f
			);
		}

		void compareBoundingBox(const AABox& expectBBox, const AABox& actualBBox, const glm::bvec3& expectPadding, float tolerance = 1e-4f) {
			for (int comp = 0; comp < 3; ++comp) {
				if (expectPadding[comp]) {
					assertFuzzyEqual(expectBBox.getInterval(comp), actualBBox.getInterval(comp), tolerance);
					Assert::IsTrue(actualBBox.getInterval(comp).size() <= 2.f * tolerance);
				}
				else {
					Assert::AreEqual(expectBBox.getInterval(comp), actualBBox.getInterval(comp));
				}
			}
		}

		void testBoundingBoxHelper(const glm::vec3& corner, const glm::vec3& side1, const glm::vec3& side2, const AABox& expectBBox, const glm::bvec3& expectPadding, float tolerance = 1.1e-4f) {
			{
				Quad quad(corner, side1, side2, dummyMaterial);
				AABox bbox = quad.boundingBox();
				compareBoundingBox(expectBBox, bbox, expectPadding, tolerance);
			}

			// inverted normal
			{
				Quad quad(corner, side2, side1, dummyMaterial);
				AABox bbox = quad.boundingBox();
				compareBoundingBox(expectBBox, bbox, expectPadding, tolerance);
			}
		}

		TEST_METHOD(TestBoundingBox)
		{
			// axis-aligned quads
			testBoundingBoxHelper(
				glm::vec3(3.f),
				glm::vec3(1.f, 0.f, 0.f),
				glm::vec3(0.f, 2.f, 0.f),
				AABox({ 3.f, 4.f }, { 3.f, 5.f }, { 3.f, 3.f }),
				glm::bvec3(false, false, true)
			);
			testBoundingBoxHelper(
				glm::vec3(-1.f, 200.f, 0.f),
				glm::vec3(2.f, 0.f, 0.f),
				glm::vec3(0.f, 0.f, 2.f),
				AABox({ -1.f, 1.f }, { 200.f, 200.f }, { 0.f, 2.f }),
				glm::bvec3(false, true, false)
			);
			testBoundingBoxHelper(
				glm::vec3(10.f, 10.f, -10.f),
				glm::vec3(0.f, 10.f, 0.f),
				glm::vec3(0.f, 0.f, -2.f),
				AABox({ 10.f, 10.f }, { 10.f, 20.f }, { -12.f, -10.f }),
				glm::bvec3(true, false, false)
			);

			// various other quads
			testBoundingBoxHelper(
				glm::vec3(0.f),
				glm::vec3(2.f, 1.f, 2.f),
				glm::vec3(1.f, 5.f, -1.f),
				AABox({ 0.f, 3.f }, { 0.f, 6.f }, { -1.f, 2.f }),
				glm::bvec3(false)
			);
			testBoundingBoxHelper(
				glm::vec3(0.f, 4.f, 2.f),
				glm::vec3(3.f, -1.f, -1.f),
				glm::vec3(0.5f, 2.f, -2.f),
				AABox({ 0.f, 3.5f }, { 3.f, 6.f }, { -1.f, 2.f }),
				glm::bvec3(false)
			);
			testBoundingBoxHelper(
				glm::vec3(-10.f, -20.f, -30.f),
				glm::vec3(-1.f, 10.f, 10.f),
				glm::vec3(100.f, -8.f, -3.f),
				AABox({ -11.f, 90.f }, { -28.f, -10.f }, { -33.f, -20.f }),
				glm::bvec3(false)
			);
		}

		template<size_t n>
		void testHitHelper(const Quad& quad, const std::array<glm::vec2, n>& uvs, const std::array<glm::vec3, n>& rayDirs, bool testTScales, float tolerance) {
			for (int i = 0; i < n; ++i) {
				const glm::vec2& uv = uvs[i];
				const glm::vec3& rayDir = rayDirs[i];
				const glm::vec3 point = quad.corner() + quad.side1() * uv.x + quad.side2() * uv.y;

				for (float sign = -1.f; sign <= 1.f; sign += 2.f) {
					const Ray ray(point - rayDir * sign, rayDir * sign);

					if (glm::all(glm::greaterThanEqual(uv, glm::vec2(0.f)))
						&& glm::all(glm::lessThanEqual(uv, glm::vec2(1.f)))) {
						
						Hittable::HitRecord expectHit;
						expectHit.t = 1.f;
						expectHit.point = point;
						expectHit.normal = sign * quad.planeNormal();
						expectHit.frontFace = sign == 1.f;
						expectHit.uv = uv;
						expectHit.material = quad.material();

						// expect no other hit, so set safeTDist to an arbitrary 0.5f
						if (testTScales) {
							testHitTScalesAndTRange(quad, ray, expectHit, 1.f, 0.5f, tolerance);
						}
						else {
							testHitTRange(quad, ray, expectHit, 1.f, 0.5f, tolerance);
						}
					}
					else {
						assertMiss(quad, ray, Interval::positive);
					}
				}
			}
		}

		TEST_METHOD(TestHit)
		{
			{
				const std::array<glm::vec2, 10> uvs = {
					glm::vec2(-0.3f, -0.3f),
					glm::vec2(-1.2f, 0.4f),
					glm::vec2(-0.1f, 1.3f),
					glm::vec2(0.5f, -1.5f),
					glm::vec2(0.4f, 0.9f),
					glm::vec2(0.7f, 0.1f),
					glm::vec2(0.8f, 2.2f),
					glm::vec2(1.1f, -0.1f),
					glm::vec2(1.4f, 0.8f),
					glm::vec2(2.5f, 1.1f),
				};

				{
					const std::array<glm::vec3, 10> rayDirs = {
						-glm::vec3(1.f, 2.f, 3.f),
						-glm::vec3(-2.f, 2.f, -1.f),
						-glm::vec3(-1.f, 3.f, 0.f),
						-glm::vec3(3.f, 2.f, 1.f),
						-glm::vec3(3.f, 2.f, 1.f),
						-glm::vec3(1.f, -0.5f, 1.f),
						-glm::vec3(-1.f, 2.f, 1.f),
						-glm::vec3(-3.f, 0.f, 0.f),
						-glm::vec3(2.f, 6.f, -1.f),
						-glm::vec3(0.f, 1.f, 1.f),
					};
					testHitHelper(Quad(
						glm::vec3(0.f), 
						glm::vec3(3.f, 1.f, 1.f),
						glm::vec3(-5.f, -1.f, -2.f),
					dummyMaterial), uvs, rayDirs, true, 1e-4f);
				}

				{
					const std::array<glm::vec3, 10> rayDirs = {
						-glm::vec3(0.f, 2.f, 0.f),
						-glm::vec3(-1.f, 3.f, 1.f),
						-glm::vec3(4.f, 3.f, 2.f),
						-glm::vec3(10.f, 30.f, -2.f),
						-glm::vec3(10.f, 1.f, -2.f),
						-glm::vec3(-3.f, 2.f, -1.f),
						-glm::vec3(0.f, 3.f, -1.f),
						-glm::vec3(1.f, 0.f, 0.f),
						-glm::vec3(4.f, 2.f, -5.f),
						-glm::vec3(0.f, 0.f, -1.f),
					};
					testHitHelper(Quad(
						glm::vec3(-10.f, -20.f, -30.f),
						glm::vec3(-1.f, 10.f, 10.f),
						glm::vec3(100.f, -8.f, -3.f),
					dummyMaterial), uvs, rayDirs, true, 1e-4f);
				}
			}

			// ray origin exactly on quad
			{
				const Quad quad(glm::vec3(1.f, 0.f, -2.f), glm::vec3(1.f), glm::vec3(-1.f, -1.f, 2.f), dummyMaterial);

				// inside quad
				assertMiss(quad, Ray(glm::vec3(1.f, 0.f, -3.f), glm::vec3(-1.f, 1.f, 1.f)), Interval::positive);
				assertMiss(quad, Ray(glm::vec3(2.f, 0.f, -3.f), glm::vec3(1.f, -1.f, 1.f)), Interval::positive);

				// on quad edges
				assertMiss(quad, Ray(glm::vec3(1.5f, 0.5f, -2.5f), glm::vec3(-1.f, 1.f, -1.f)), Interval::positive);
				assertMiss(quad, Ray(glm::vec3(1.5f, 0.5f, 0.f), glm::vec3(-1.f, 1.f, -2.f)), Interval::positive);
				assertMiss(quad, Ray(glm::vec3(0.5f, -0.5f, -1.f), glm::vec3(-1.f, 2.f, 1.f)), Interval::positive);
				assertMiss(quad, Ray(glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(-1.f, 1.f, 1.f)), Interval::positive);

				// on quad corners
				assertMiss(quad, Ray(glm::vec3(1.f, 0.f, -2.f), glm::vec3(-1.f, 2.f, 1.f)), Interval::positive);
				assertMiss(quad, Ray(glm::vec3(1.f, 0.f, 1.f), glm::vec3(1.f, -1.f, 1.f)), Interval::positive);
				assertMiss(quad, Ray(glm::vec3(0.f, 1.f, 0.f), glm::vec3(-1.f, 1.f, -2.f)), Interval::positive);
				assertMiss(quad, Ray(glm::vec3(2.f, 1.f, -1.f), glm::vec3(-1.f, 1.f, -1.f)), Interval::positive);
			}

			// hit exactly on quad edges/corners
			{
				const std::array<glm::vec2, 9> uvs = {
					glm::vec2(0.f, 0.f),
					glm::vec2(0.f, 0.5f),
					glm::vec2(0.f, 1.f),
					glm::vec2(0.5f, 0.f),
					glm::vec2(0.5f, 0.5f),
					glm::vec2(0.5f, 1.f),
					glm::vec2(1.f, 0.0f),
					glm::vec2(1.f, 0.5f),
					glm::vec2(1.f, 1.f),
				};
				const std::array<glm::vec3, 9> rayDirs = {
					-glm::vec3(-1.f, 1.f, 1.f),
					-glm::vec3(-1.f, 1.f, 1.f),
					-glm::vec3(-1.f, 1.f, 1.f),
					-glm::vec3(-1.f, 1.f, 1.f),
					-glm::vec3(-1.f, 1.f, 1.f),
					-glm::vec3(-1.f, 1.f, 1.f),
					-glm::vec3(-1.f, 1.f, 1.f),
					-glm::vec3(-1.f, 1.f, 1.f),
					-glm::vec3(-1.f, 1.f, 1.f),
				};

				// do not test t scales because then the hit may not be exact
				testHitHelper(Quad(
					glm::vec3(0.f),
					glm::vec3(1.f, -1.f, 1.f),
					glm::vec3(-1.f, 2.f, -1.f),
					dummyMaterial), uvs, rayDirs, false, 1e-4f);
			}

			// ray parallel to quad
			{
				const Quad quad(glm::vec3(-1.f, 1.f, 1.f), glm::vec3(1.f), glm::vec3(2.f, -1.f, -1.f), dummyMaterial);

				// origin not on quad, ray parallel to quad
				assertMiss(quad, Ray(glm::vec3(-1.f, 1.f, 0.f), glm::vec3(2.f)), Interval::positive);
				assertMiss(quad, Ray(glm::vec3(0.f, 1.f, 0.f), glm::vec3(1.f, 2.f, 2.f)), Interval::positive);

				// origin not on quad, ray lies along quad
				assertMiss(quad, Ray(glm::vec3(-1.f, 0.f, 0.f), glm::vec3(2.f)), Interval::positive);
				assertMiss(quad, Ray(glm::vec3(1.f, 2.f, 2.f), glm::vec3(0.f, -1.f, -1.f)), Interval::positive);

				// origin not on quad, ray lies along quad edge
				assertMiss(quad, Ray(glm::vec3(-1.5f, 0.5f, 0.5f), glm::vec3(2.f)), Interval::positive);

				// origin on quad, ray lies along quad edge
				assertMiss(quad, Ray(glm::vec3(-1.f, 1.f, 1.f), glm::vec3(2.f)), Interval::positive);
			}
		}
	};
}