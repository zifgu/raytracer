#include <iostream>
#include <limits>

#include "common.h"
#include "image.h"
#include "ray.h"
#include "sphere.h"
#include "hittable_list.h"
#include "camera.h"
#include "renderer.h"
#include "lambertian.h"
#include "metal.h"
#include "dielectric.h"

int main() {
    HittableList world;
    auto ground_material = std::make_shared<Lambertian>(glm::vec3(0.5, 0.5, 0.5));
    world.add(std::make_shared<Sphere>(glm::vec3(0, -1000, 0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random();
            glm::vec3 center(a + 0.9 * random(), 0.2, b + 0.9 * random());

            if ((center - glm::vec3(4, 0.2, 0)).length() > 0.9) {
                std::shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = glm::vec3(random(), random(), random()) * glm::vec3(random(), random(), random());
                    sphere_material = std::make_shared<Lambertian>(albedo);
                    world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                }
                else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = glm::vec3(random(0.5f, 1.f), random(0.5f, 1.f), random(0.5f, 1.f));
                    auto fuzz = random(0, 0.5);
                    sphere_material = std::make_shared<Metal>(albedo, fuzz);
                    world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                }
                else {
                    // glass
                    sphere_material = std::make_shared<Dielectric>(1.5);
                    world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = std::make_shared<Dielectric>(1.5);
    world.add(std::make_shared<Sphere>(glm::vec3(0, 1, 0), 1.0, material1));

    auto material2 = std::make_shared<Lambertian>(glm::vec3(0.4, 0.2, 0.1));
    world.add(std::make_shared<Sphere>(glm::vec3(-4, 1, 0), 1.0, material2));

    auto material3 = std::make_shared<Metal>(glm::vec3(0.7, 0.6, 0.5), 0.0);
    world.add(std::make_shared<Sphere>(glm::vec3(4, 1, 0), 1.0, material3));

	glm::ivec2 imageSize = glm::ivec2(1200, 675);
	glm::vec3 lookFrom(13, 2, 3);
	glm::vec3 lookAt(0.f, 0.f, 0.f);
	Camera::Projection projection(imageSize, 20.f, 10.f);
	Camera::Frame frame(lookFrom, lookAt);
	Camera camera(frame, projection);

	Image img(imageSize.x, imageSize.y);
	
	Renderer renderer;
	renderer.render(world, camera, img);

	img.gammaCorrect();

	img.write("C:\\Users\\markf\\GitHub\\raytracer\\render\\final.png");
}