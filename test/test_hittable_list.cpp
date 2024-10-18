#include "pch.h"
#include "CppUnitTest.h"

#include "test_common.h"
#include "test_hittable.h"
#include "../src/hittable_list.h"
#include "../src/sphere.h"
#include "../src/quad.h"
#include "../src/lambertian.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	TEST_CLASS(TestHittableList)
	{
		const std::shared_ptr<Material> dummyMaterial = std::make_shared<Lambertian>(glm::vec3(1.f));
	public:
		TEST_METHOD(TestConstructor)
		{
			HittableList hittables;
			Assert::AreEqual(0, static_cast<int>(hittables.objects().size()));
			Assert::AreEqual(AABox::empty, hittables.boundingBox());
		}
		
		void assertHittableListContains(const HittableList& hittables, const std::shared_ptr<Hittable>& hittable) {
			bool contains = false;
			for (const std::shared_ptr<Hittable>& obj : hittables.objects()) {
				contains = contains || (obj.get() == hittable.get());
			}
			Assert::IsTrue(contains);
		}
		
		TEST_METHOD(TestAdd_BoundingBox_Clear)
		{
			HittableList hittables;

			std::shared_ptr<Hittable> sphere = std::make_shared<Sphere>(glm::vec3(0.f), 1.f, dummyMaterial);
			hittables.add(sphere);
			Assert::AreEqual(1, static_cast<int>(hittables.objects().size()));
			Assert::AreEqual(AABox({ -1.f, 1.f }, { -1.f, 1.f }, {-1.f, 1.f}), hittables.boundingBox());
			assertHittableListContains(hittables, sphere);

			std::shared_ptr<Hittable> quad = std::make_shared<Quad>(glm::vec3(0.f), glm::vec3(2.f, 0.f, 0.f), glm::vec3(-2.f), dummyMaterial);
			hittables.add(quad);
			Assert::AreEqual(2, static_cast<int>(hittables.objects().size()));
			Assert::AreEqual(AABox({ -2.f, 2.f }, { -2.f, 1.f }, { -2.f, 1.f }), hittables.boundingBox());
			assertHittableListContains(hittables, sphere);
			assertHittableListContains(hittables, quad);

			std::shared_ptr<Hittable> sphere2 = std::make_shared<Sphere>(glm::vec3(-5.f), 2.f, dummyMaterial);
			hittables.add(sphere2);
			Assert::AreEqual(3, static_cast<int>(hittables.objects().size()));
			Assert::AreEqual(AABox({ -7.f, 2.f }, { -7.f, 1.f }, { -7.f, 1.f }), hittables.boundingBox());
			assertHittableListContains(hittables, sphere2);

			hittables.clear();
			Assert::AreEqual(0, static_cast<int>(hittables.objects().size()));
			Assert::AreEqual(AABox::empty, hittables.boundingBox());

			hittables.add(sphere2);
			Assert::AreEqual(1, static_cast<int>(hittables.objects().size()));
			Assert::AreEqual(AABox({ -7.f, -3.f }, { -7.f, -3.f }, { -7.f, -3.f }), hittables.boundingBox());
			assertHittableListContains(hittables, sphere2);
		}

		HittableList createTestScene(std::shared_ptr<Hittable>& sphere, std::shared_ptr<Hittable>& quad, bool reverseAddOrder = false) {
			HittableList hittables;
			hittables.clear();

			std::shared_ptr<Material> sphereMaterial = std::make_shared<Lambertian>(glm::vec3(1.f, 0.f, 0.f));
			std::shared_ptr<Material> quadMaterial = std::make_shared<Lambertian>(glm::vec3(0.f, 1.f, 0.f));

			sphere = std::make_shared<Sphere>(glm::vec3(0.f), 1.f, sphereMaterial);
			quad = std::make_shared<Quad>(glm::vec3(0.f), glm::vec3(2.f, 0.f, 0.f), glm::vec3(-2.f), quadMaterial);

			if (!reverseAddOrder) {
				hittables.add(sphere);
				hittables.add(quad);
			}
			else {
				hittables.add(quad);
				hittables.add(sphere);
			}

			return hittables;
		}

		TEST_METHOD(TestHit)
		{
			// hits no objects
			{
				std::shared_ptr<Hittable> sphere = nullptr, quad = nullptr;
				const HittableList hittables = createTestScene(sphere, quad);

				assertMiss(hittables, Ray(glm::vec3(0.f, -2.f, 1.f), glm::vec3(0.f, 3.f, 1.f)), Interval::positive);
				assertMiss(hittables, Ray(glm::vec3(1.f, -2.f, 1.f), glm::vec3(0.f, 2.f, 0.f)), Interval::positive);
				assertMiss(hittables, Ray(glm::vec3(0.f, -2.f, -1.f), glm::vec3(-2.f, 3.f, 2.f)), Interval::positive);
				assertMiss(hittables, Ray(glm::vec3(0.5f, 1.f, 1.f), glm::vec3(2.f, -3.f, -2.f)), Interval::positive);
			}

			// hits one object
			{
				std::shared_ptr<Hittable> sphere = nullptr, quad = nullptr;
				const HittableList hittables = createTestScene(sphere, quad);

				// hits sphere
				{
					Hittable::HitRecord expectHit;
					Ray ray(glm::vec3(1.f), glm::vec3(-1.f, -2.f, -1.f));
					Assert::IsTrue(sphere->hit(ray, Interval::positive, expectHit));
					assertMiss(*quad, ray, Interval::positive);

					testHitTScalesAndTRange(hittables, ray, expectHit, expectHit.t, 0.5f, 1e-4f);
				}
				{
					Hittable::HitRecord expectHit;
					Ray ray(glm::vec3(0.f, -3.f, -3.f), glm::vec3(0.f, 2.f, 2.f));
					Assert::IsTrue(sphere->hit(ray, Interval::positive, expectHit));
					assertMiss(*quad, ray, Interval::positive);

					testHitTScalesAndTRange(hittables, ray, expectHit, expectHit.t, 0.5f, 1e-4f);
				}

				// hits quad
				{
					Hittable::HitRecord expectHit;
					Ray ray(glm::vec3(0.f, 2.f, -0.5f), glm::vec3(-1.f, -4.f, -1.f));
					assertMiss(*sphere, ray, Interval::positive);
					Assert::IsTrue(quad->hit(ray, Interval::positive, expectHit));

					testHitTScalesAndTRange(hittables, ray, expectHit, expectHit.t, 0.5f, 1e-4f);
				}
				{
					Hittable::HitRecord expectHit;
					Ray ray(glm::vec3(1.f, -2.f, 0.f), glm::vec3(-1.f, 1.f, -1.f));
					assertMiss(*sphere, ray, Interval::positive);
					Assert::IsTrue(quad->hit(ray, Interval::positive, expectHit));

					testHitTScalesAndTRange(hittables, ray, expectHit, expectHit.t, 0.5f, 1e-4f);
				}
			}

			// hits both objects - should get back closer hit
			{
				std::shared_ptr<Hittable> sphere = nullptr, quad = nullptr;
				const HittableList hittables = createTestScene(sphere, quad);

				// hits sphere first
				{
					Ray ray(glm::vec3(0.f, -1.f, 2.f), glm::vec3(1.f, 1.f, -3.f));
					Hittable::HitRecord ignore;
					Hittable::HitRecord expectHit;
					Assert::IsTrue(quad->hit(ray, Interval::positive, ignore));
					Assert::IsTrue(sphere->hit(ray, Interval::positive, expectHit));

					testHitTScalesAndTRange(hittables, ray, expectHit, expectHit.t, 0.25f, 1e-4f);
				}
				{
					Ray ray(glm::vec3(0.f, 2.f, 0.f), glm::vec3(1.f, -4.f, -1.f));
					Hittable::HitRecord ignore;
					Hittable::HitRecord expectHit;
					Assert::IsTrue(quad->hit(ray, Interval::positive, ignore));
					Assert::IsTrue(sphere->hit(ray, Interval::positive, expectHit));

					testHitTScalesAndTRange(hittables, ray, expectHit, expectHit.t, 0.25f, 1e-4f);
				}
				{
					Ray ray(glm::vec3(0.f, 0.5f, -0.5f), glm::vec3(0.f, -2.f, -1.f));
					Hittable::HitRecord ignore;
					Hittable::HitRecord expectHit;
					Assert::IsTrue(quad->hit(ray, Interval::positive, ignore));
					Assert::IsTrue(sphere->hit(ray, Interval::positive, expectHit));

					testHitTScalesAndTRange(hittables, ray, expectHit, expectHit.t, 0.25f, 1e-4f);
				}

				// hits quad first
				{
					Ray ray(glm::vec3(1.f, -1.f, -2.f), glm::vec3(-1.f, 1.f, 6.f));
					Hittable::HitRecord ignore;
					Hittable::HitRecord expectHit;
					Assert::IsTrue(sphere->hit(ray, Interval::positive, ignore));
					Assert::IsTrue(quad->hit(ray, Interval::positive, expectHit));

					testHitTScalesAndTRange(hittables, ray, expectHit, expectHit.t, 0.1f, 1e-4f);
				}
				{
					Ray ray(glm::vec3(0.f, -2.f, -1.f), glm::vec3(0.f, 1.f, 0.f));
					Hittable::HitRecord ignore;
					Hittable::HitRecord expectHit;
					Assert::IsTrue(sphere->hit(ray, Interval::positive, ignore));
					Assert::IsTrue(quad->hit(ray, Interval::positive, expectHit));

					testHitTScalesAndTRange(hittables, ray, expectHit, expectHit.t, 0.5f, 1e-4f);
				}
				{
					Ray ray(glm::vec3(0.f, 0.5f, -0.5f), glm::vec3(0.f, -2.f, 1.f));
					Hittable::HitRecord ignore;
					Hittable::HitRecord expectHit;
					Assert::IsTrue(sphere->hit(ray, Interval::positive, ignore));
					Assert::IsTrue(quad->hit(ray, Interval::positive, expectHit));

					testHitTScalesAndTRange(hittables, ray, expectHit, expectHit.t, 0.25f, 1e-4f);
				}
			}

			// hits 2 objects at the exact same point - should return the first one added
			{
				std::shared_ptr<Hittable> sphere = nullptr, quad = nullptr;
				const HittableList hittables = createTestScene(sphere, quad);

				Ray ray(glm::vec3(1.f, -1.f, 1.f), glm::vec3(0.f, 1.f, -1.f));
				Hittable::HitRecord sphereHit;
				Hittable::HitRecord quadHit;
				Assert::IsTrue(sphere->hit(ray, Interval::positive, sphereHit));
				Assert::IsTrue(quad->hit(ray, Interval::positive, quadHit));
				Assert::AreEqual(sphereHit.t, quadHit.t);
				Assert::AreEqual(sphereHit.point, quadHit.point);

				testHitTRange(hittables, ray, sphereHit, sphereHit.t, 0.5f, 1e-4f, true);
			}
			{
				std::shared_ptr<Hittable> sphere = nullptr, quad = nullptr;
				const HittableList hittables = createTestScene(sphere, quad, true);

				Ray ray(glm::vec3(1.f, -1.f, 1.f), glm::vec3(0.f, 1.f, -1.f));
				Hittable::HitRecord sphereHit;
				Hittable::HitRecord quadHit;
				Assert::IsTrue(sphere->hit(ray, Interval::positive, sphereHit));
				Assert::IsTrue(quad->hit(ray, Interval::positive, quadHit));
				Assert::AreEqual(sphereHit.t, quadHit.t);
				Assert::AreEqual(sphereHit.point, quadHit.point);

				testHitTRange(hittables, ray, quadHit, quadHit.t, 0.5f, 1e-4f);
			}
		}
	};
}