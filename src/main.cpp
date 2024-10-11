#include <iostream>
#include <limits>

#include "common.h"
#include "image.h"
#include "ray.h"
#include "sphere.h"
#include "hittable_list.h"
#include "projection.h"
#include "renderer.h"
#include "lambertian.h"
#include "metal.h"
#include "dielectric.h"

int main() {
	std::shared_ptr<Material> groundMaterial = std::make_shared<Lambertian>(glm::vec3(0.8f, 0.8f, 0.0f));
	std::shared_ptr<Material> centerSphereMaterial = std::make_shared<Lambertian>(glm::vec3(0.1f, 0.2f, 0.5f));
	std::shared_ptr<Material> leftSphereMaterial = std::make_shared<Dielectric>(1.5f);
	std::shared_ptr<Material> leftInnerSphereMaterial = std::make_shared<Dielectric>(1.f / 1.5f);
	std::shared_ptr<Material> rightSphereMaterial = std::make_shared<Metal>(glm::vec3(0.8f, 0.6f, 0.2f), 0.0f);

	HittableList world;
	world.add(std::make_shared<Sphere>(glm::vec3(0.0f, -100.5f, -1.0f), 100.0f, groundMaterial));
	world.add(std::make_shared<Sphere>(glm::vec3(0.0f, 0.0f, -1.2f), 0.5f, centerSphereMaterial));
	world.add(std::make_shared<Sphere>(glm::vec3(-1.0f, 0.0f, -1.0f), 0.5f, leftSphereMaterial));
	world.add(std::make_shared<Sphere>(glm::vec3(-1.0f, 0.0f, -1.0f), 0.4f, leftInnerSphereMaterial));
	world.add(std::make_shared<Sphere>(glm::vec3(1.0f, 0.0f, -1.0f), 0.5f, rightSphereMaterial));

	glm::ivec2 imageSize = glm::ivec2(320, 180);
	Projection projection(imageSize, glm::vec2(3.55555556f, 2.0f), 1.f);

	Image img(imageSize.x, imageSize.y);
	
	Renderer renderer;
	renderer.render(world, projection, img);

	img.gammaCorrect();

	img.write("C:\\Users\\markf\\GitHub\\raytracer\\render\\test.png");
}