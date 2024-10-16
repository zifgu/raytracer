#include <iostream>
#include <limits>

#include "common.h"
#include "image.h"
#include "sphere.h"
#include "hittable_list.h"
#include "camera.h"
#include "renderer.h"
#include "lambertian.h"
#include "metal.h"
#include "dielectric.h"
#include "bvh.h"

HittableList testEarthScene() {
    HittableList world;

    std::shared_ptr<Texture> earthTexture = std::make_shared<ImageTexture>("C:\\Users\\markf\\GitHub\\raytracer\\data\\earthmap.jpg");
    auto earthMaterial = std::make_shared<Lambertian>(earthTexture);
    auto globe = std::make_shared<Sphere>(glm::vec3(0.f, 0.f, 0.f), 2.f, earthMaterial);

    world.add(globe);
    return world;
}

Camera testEarthSceneCamera(const glm::ivec2& imageSize) {
    glm::vec3 lookFrom(0.f, 0.f, 12.f);
    glm::vec3 lookAt(0.f, 0.f, 0.f);
    Camera::Projection projection(imageSize, 20.f, 1.f);
    Camera::Frame frame(lookFrom, lookAt);
    return Camera(frame, projection);
}

HittableList testCheckeredSpheresScene() {
    HittableList world;

    std::shared_ptr<Texture> checkerEven = std::make_shared<SolidColorTexture>(glm::vec3(0.2f, 0.3f, 0.1f));
    std::shared_ptr<Texture> checkerOdd = std::make_shared<SolidColorTexture>(glm::vec3(0.9f, 0.9f, 0.9f));
    std::shared_ptr<Texture> checker = std::make_shared<CheckerTexture>(0.32f, checkerEven, checkerOdd);

    world.add(std::make_shared<Sphere>(glm::vec3(0.f, -10.f, 0.f), 10.f, std::make_shared<Lambertian>(checker)));
    world.add(std::make_shared<Sphere>(glm::vec3(0.f, 10.f, 0.f), 10.f, std::make_shared<Lambertian>(checker)));

    return world;
}

Camera testCheckeredSpheresSceneCamera(const glm::ivec2& imageSize) {
    glm::vec3 lookFrom(13.f, 2.f, 3.f);
    glm::vec3 lookAt(0.f, 0.f, 0.f);
    Camera::Projection projection(imageSize, 20.f, 1.f);
    Camera::Frame frame(lookFrom, lookAt);
    return Camera(frame, projection);
}

HittableList testScene() {
    HittableList world;
    std::shared_ptr<Texture> checkerEven = std::make_shared<SolidColorTexture>(glm::vec3(0.2f, 0.3f, 0.1f));
    std::shared_ptr<Texture> checkerOdd = std::make_shared<SolidColorTexture>(glm::vec3(0.9f, 0.9f, 0.9f));
    std::shared_ptr<Texture> checker = std::make_shared<CheckerTexture>(0.32f, checkerEven, checkerOdd);

    auto material_ground = std::make_shared<Lambertian>(checker);
    auto material_center = std::make_shared<Lambertian>(glm::vec3(0.1f, 0.2f, 0.5f));
    auto material_left = std::make_shared<Dielectric>(1.5f);
    auto material_bubble = std::make_shared<Dielectric>(1.f / 1.5f);
    auto material_right = std::make_shared<Metal>(glm::vec3(0.8f, 0.6f, 0.2f), 1.f);

    world.add(std::make_shared<Sphere>(glm::vec3(0.f, -100.5f, -1.f), 100.f, material_ground));
    world.add(std::make_shared<Sphere>(glm::vec3(0.f, 0.f, -1.2f), 0.5f, material_center));
    world.add(std::make_shared<Sphere>(glm::vec3(-1.f, 0.f, -1.f), 0.5f, material_left));
    world.add(std::make_shared<Sphere>(glm::vec3(-1.f, 0.f, -1.f), 0.4f, material_bubble));
    world.add(std::make_shared<Sphere>(glm::vec3(1.f, 0.f, -1.f), 0.5f, material_right));

    return world;
}

Camera testSceneCamera(const glm::ivec2& imageSize) {
    glm::vec3 lookFrom(0.f, 0.f, 0.f);
    glm::vec3 lookAt(0.f, 0.f, -1.f);
    Camera::Projection projection(imageSize, 90.f, 1.f);
    Camera::Frame frame(lookFrom, lookAt);
    return Camera(frame, projection);
}

HittableList finalScene() {
    HittableList world;
    auto ground_material = std::make_shared<Lambertian>(glm::vec3(0.5f, 0.5f, 0.5f));
    world.add(std::make_shared<Sphere>(glm::vec3(0.f, -1000.f, 0.f), 1000.f, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            float choose_mat = random();
            glm::vec3 center(a + 0.9f * random(), 0.2f, b + 0.9f * random());

            if ((center - glm::vec3(4.f, 0.2f, 0.f)).length() > 0.9f) {
                std::shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8f) {
                    // diffuse
                    auto albedo = glm::vec3(random(), random(), random()) * glm::vec3(random(), random(), random());
                    sphere_material = std::make_shared<Lambertian>(albedo);
                    world.add(std::make_shared<Sphere>(center, 0.2f, sphere_material));
                }
                else if (choose_mat < 0.95f) {
                    // metal
                    auto albedo = glm::vec3(random(0.5f, 1.f), random(0.5f, 1.f), random(0.5f, 1.f));
                    auto fuzz = random(0.f, 0.5f);
                    sphere_material = std::make_shared<Metal>(albedo, fuzz);
                    world.add(std::make_shared<Sphere>(center, 0.2f, sphere_material));
                }
                else {
                    // glass
                    sphere_material = std::make_shared<Dielectric>(1.5f);
                    world.add(std::make_shared<Sphere>(center, 0.2f, sphere_material));
                }
            }
        }
    }

    auto material1 = std::make_shared<Dielectric>(1.5f);
    world.add(std::make_shared<Sphere>(glm::vec3(0.f, 1.f, 0.f), 1.f, material1));

    auto material2 = std::make_shared<Lambertian>(glm::vec3(0.4f, 0.2f, 0.1f));
    world.add(std::make_shared<Sphere>(glm::vec3(-4.f, 1.f, 0.f), 1.f, material2));

    auto material3 = std::make_shared<Metal>(glm::vec3(0.7f, 0.6f, 0.5f), 0.f);
    world.add(std::make_shared<Sphere>(glm::vec3(4.f, 1.f, 0.f), 1.f, material3));

    return world;
}

Camera finalSceneCamera(const glm::ivec2& imageSize) {
    glm::vec3 lookFrom(13.f, 2.f, 3.f);
    glm::vec3 lookAt(0.f, 0.f, 0.f);
    Camera::Projection projection(imageSize, 20.f, 10.f);
    Camera::Frame frame(lookFrom, lookAt);
    return Camera(frame, projection);
}

int main() {
    glm::ivec2 imageSize = glm::ivec2(320, 180);
    HittableList world = finalScene();
    Camera camera = finalSceneCamera(imageSize);
    Image img(imageSize.x, imageSize.y);

    HittableList bvhWorld;
    bvhWorld.add(std::make_shared<BVHNode>(world.objects(), 0, world.objects().size()));
	
	Renderer renderer;
	renderer.render(bvhWorld, camera, img);

	img.gammaCorrect();

	img.write("C:\\Users\\markf\\GitHub\\raytracer\\render\\test.png");
}