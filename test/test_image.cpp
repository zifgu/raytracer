#include "pch.h"
#include "CppUnitTest.h"

#include "test_common.h"
#include "../src/image.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	TEST_CLASS(TestImage)
	{
	public:

		TEST_METHOD(TestConstructor)
		{
			// empty image
			{
				const int width = 90, height = 160;
				Image image(width, height);
				Assert::AreEqual(width, image.width());
				Assert::AreEqual(height, image.height());
				for (int x = 0; x < width; ++x) {
					for (int y = 0; y < height; ++y) {
						Assert::AreEqual(glm::vec3(0.f), image.get(x, y));
					}
				}
			}

			// from nonexistent file
			{
				Image image("C:\\Users\\markf\\GitHub\\raytracer\\data\\test_nonexistent.png");
				Assert::AreEqual(0, image.width());
				Assert::AreEqual(0, image.height());
			}

			// TODO: avoid using absolute path on my machine
			const std::string path = "C:\\Users\\markf\\GitHub\\raytracer\\data\\test.png";
			const float tolerance = 0.01f;

			// from file, without gamma decoding
			{
				Image image(path, 1.f);
				Assert::AreEqual(85, image.width());
				Assert::AreEqual(80, image.height());

				Assert::AreEqual(glm::vec3(0.f), image.get(0, 0));
				Assert::AreEqual(glm::vec3(0.f), image.get(84, 0));
				Assert::AreEqual(glm::vec3(0.f), image.get(0, 14));
				Assert::AreEqual(glm::vec3(1.f), image.get(0, 15));
				Assert::AreEqual(glm::vec3(0.f), image.get(43, 14));
				Assert::AreEqual(glm::vec3(1.f), image.get(43, 15));
				assertFuzzyEqual(glm::vec3(0.5f, 0.f, 1.f), image.get(44, 15), tolerance);
				Assert::AreEqual(glm::vec3(1.f, 0.f, 0.f), image.get(45, 15));
				Assert::AreEqual(glm::vec3(1.f, 0.f, 0.f), image.get(63, 15));
				assertFuzzyEqual(glm::vec3(0.5f, 0.f, 1.f), image.get(64, 15), tolerance);
				Assert::AreEqual(glm::vec3(1.f), image.get(65, 15));
				Assert::AreEqual(glm::vec3(1.f, 1.f, 0.f), image.get(39, 42));
				assertFuzzyEqual(glm::vec3(0.f, 0.5f, 1.f), image.get(40, 42), tolerance);
				assertFuzzyEqual(glm::vec3(0.5f, 1.f, 0.f), image.get(83, 55), tolerance);
				assertFuzzyEqual(glm::vec3(0.f, 0.5f, 1.f), image.get(0, 79), tolerance);
				assertFuzzyEqual(glm::vec3(0.f, 0.5f, 1.f), image.get(84, 79), tolerance);
			}

			// from file, with gamma decoding
			{
				Image image(path);
				Assert::AreEqual(85, image.width());
				Assert::AreEqual(80, image.height());

				Assert::AreEqual(glm::vec3(0.f), image.get(0, 0));
				Assert::AreEqual(glm::vec3(0.f), image.get(84, 0));
				Assert::AreEqual(glm::vec3(0.f), image.get(0, 14));
				Assert::AreEqual(glm::vec3(1.f), image.get(0, 15));
				Assert::AreEqual(glm::vec3(0.f), image.get(43, 14));
				Assert::AreEqual(glm::vec3(1.f), image.get(43, 15));
				assertFuzzyEqual(glm::vec3(0.216f, 0.f, 1.f), image.get(44, 15), tolerance);
				Assert::AreEqual(glm::vec3(1.f, 0.f, 0.f), image.get(45, 15));
				Assert::AreEqual(glm::vec3(1.f, 0.f, 0.f), image.get(63, 15));
				assertFuzzyEqual(glm::vec3(0.216f, 0.f, 1.f), image.get(64, 15), tolerance);
				Assert::AreEqual(glm::vec3(1.f), image.get(65, 15));
				Assert::AreEqual(glm::vec3(1.f, 1.f, 0.f), image.get(39, 42));
				assertFuzzyEqual(glm::vec3(0.f, 0.216f, 1.f), image.get(40, 42), tolerance);
				assertFuzzyEqual(glm::vec3(0.216f, 1.f, 0.f), image.get(83, 55), tolerance);
				assertFuzzyEqual(glm::vec3(0.f, 0.216f, 1.f), image.get(0, 79), tolerance);
				assertFuzzyEqual(glm::vec3(0.f, 0.216f, 1.f), image.get(84, 79), tolerance);
			}
		}

		TEST_METHOD(TestSet)
		{
			const int width = 85, height = 80;
			Image image("C:\\Users\\markf\\GitHub\\raytracer\\data\\test.png");
			Assert::AreEqual(width, image.width());
			Assert::AreEqual(height, image.height());

			const Image copy = image;

			const int setX = 45, setY = 29;
			image.set(setX, setY, glm::vec3(0.1f));

			for (int x = 0; x < width; ++x) {
				for (int y = 0; y < height; ++y) {
					if (x == setX && y == setY) {
						Assert::AreEqual(glm::vec3(0.1f), image.get(x, y));
					}
					else {
						Assert::AreEqual(copy.get(x, y), image.get(x, y));
					}
				}
			}
		}

		TEST_METHOD(TestWrite)
		{
			// TODO: avoid using absolute path on my machine
			const std::string path = "C:\\Users\\markf\\GitHub\\raytracer\\data\\testWrite.png";
			const float tolerance = 0.01f;
			
			const int width = 30, height = 45;
			// write
			{
				Image image(width, height);
				image.set(29, 0, glm::vec3(1.f, 0.f, 0.f));
				image.set(16, 3, glm::vec3(0.f, 1.f, 0.5f));
				image.set(10, 44, glm::vec3(0.5f, 0.5f, 0.f));
				image.write(path);
			}

			// read, without gamma decoding
			{
				Image image(path, 1.f);
				Assert::AreEqual(width, image.width());
				Assert::AreEqual(height, image.height());
				assertFuzzyEqual(glm::vec3(1.f, 0.f, 0.f), image.get(29, 0), tolerance);
				assertFuzzyEqual(glm::vec3(0.f, 1.f, 0.73f), image.get(16, 3), tolerance);
				assertFuzzyEqual(glm::vec3(0.73f, 0.73f, 0.f), image.get(10, 44), tolerance);
			}

			// read, with gamma decoding
			{
				Image image(path);
				Assert::AreEqual(width, image.width());
				Assert::AreEqual(height, image.height());
				assertFuzzyEqual(glm::vec3(1.f, 0.f, 0.f), image.get(29, 0), tolerance);
				assertFuzzyEqual(glm::vec3(0.f, 1.f, 0.5f), image.get(16, 3), tolerance);
				assertFuzzyEqual(glm::vec3(0.5f, 0.5f, 0.f), image.get(10, 44), tolerance);
			}
		}
	};
}