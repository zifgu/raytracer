#include <iostream>
#include <limits>

#include "common.h"
#include "image.h"
#include "sphere.h"
#include "quad.h"
#include "hittable_list.h"
#include "camera.h"
#include "renderer.h"
#include "lambertian.h"
#include "metal.h"
#include "dielectric.h"
#include "emissive.h"
#include "bvh.h"

HittableList cornellBoxScene() {
    HittableList world;

    auto red = std::make_shared<Lambertian>(glm::vec3(.65f, .05f, .05f));
    auto white = std::make_shared<Lambertian>(glm::vec3(.73f, .73f, .73f));
    auto green = std::make_shared<Lambertian>(glm::vec3(.12f, .45f, .15f));
    auto light = std::make_shared<DiffuseEmissive>(glm::vec3(15.f, 15.f, 15.f));

    world.add(std::make_shared<Quad>(glm::vec3(555.f, 0.f, 0.f), glm::vec3(0.f, 555.f, 0.f), glm::vec3(0.f, 0.f, 555.f), green));
    world.add(std::make_shared<Quad>(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 555.f, 0.f), glm::vec3(0.f, 0.f, 555.f), red));
    world.add(std::make_shared<Quad>(glm::vec3(343.f, 554.f, 332.f), glm::vec3(-130.f, 0.f, 0.f), glm::vec3(0.f, 0.f, -105.f), light));
    world.add(std::make_shared<Quad>(glm::vec3(0.f, 0.f, 0.f), glm::vec3(555.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 555.f), white));
    world.add(std::make_shared<Quad>(glm::vec3(555.f, 555.f, 555.f), glm::vec3(-555.f, 0.f, 0.f), glm::vec3(0.f, 0.f, -555.f), white));
    world.add(std::make_shared<Quad>(glm::vec3(0.f, 0.f, 555.f), glm::vec3(555.f, 0.f, 0.f), glm::vec3(0.f, 555.f, 0.f), white));

    return world;
}

Camera cornellBoxSceneCamera(const glm::ivec2& imageSize) {
    glm::vec3 lookFrom(278.f, 278.f, -800.f);
    glm::vec3 lookAt(278.f, 278.f, 0.f);
    Camera::Projection projection(imageSize, 40.f, 1.f);
    Camera::Frame frame(lookFrom, lookAt);
    return Camera(frame, projection);
}

HittableList testLightsScene() {
    HittableList world;

    auto ground_material = std::make_shared<Lambertian>(glm::vec3(0.5f, 0.5f, 0.5f));
    world.add(std::make_shared<Sphere>(glm::vec3(0.f, -1000.f, 0.f), 1000.f, ground_material));
    world.add(std::make_shared<Sphere>(glm::vec3(0.f, 2.f, 0.f), 2.f, ground_material));

    auto difflight = std::make_shared<DiffuseEmissive>(glm::vec3(4.f, 4.f, 4.f));
    world.add(std::make_shared<Quad>(glm::vec3(3.f, 1.f, -2.f), glm::vec3(2.f, 0.f, 0.f), glm::vec3(0.f, 2.f, 0.f), difflight));
    world.add(std::make_shared<Sphere>(glm::vec3(0.f, 7.f, 0.f), 2.f, difflight));

    return world;
}

Camera testLightsSceneCamera(const glm::ivec2& imageSize) {
    glm::vec3 lookFrom(26.f, 3.f, 6.f);
    glm::vec3 lookAt(0.f, 2.f, 0.f);
    Camera::Projection projection(imageSize, 20.f, 1.f);
    Camera::Frame frame(lookFrom, lookAt);
    return Camera(frame, projection);
}

HittableList testQuadScene() {
    HittableList world;

    // Materials
    auto left_red = std::make_shared<Lambertian>(glm::vec3(1.0f, 0.2f, 0.2f));
    auto back_green = std::make_shared<Lambertian>(glm::vec3(0.2f, 1.0f, 0.2f));
    auto right_blue = std::make_shared<Lambertian>(glm::vec3(0.2f, 0.2f, 1.0f));
    auto upper_orange = std::make_shared<Lambertian>(glm::vec3(1.0f, 0.5f, 0.0f));
    auto lower_teal = std::make_shared<Lambertian>(glm::vec3(0.2f, 0.8f, 0.8f));

    // Quads
    world.add(std::make_shared<Quad>(glm::vec3(-3.f, -2.f, 5.f), glm::vec3(0.f, 0.f, -4.f), glm::vec3(0.f, 4.f, 0.f), left_red));
    world.add(std::make_shared<Quad>(glm::vec3(-2.f, -2.f, 0.f), glm::vec3(4.f, 0.f, 0.f), glm::vec3(0.f, 4.f, 0.f), back_green));
    world.add(std::make_shared<Quad>(glm::vec3(3.f, -2.f, 1.f), glm::vec3(0.f, 0.f, 4.f), glm::vec3(0.f, 4.f, 0.f), right_blue));
    world.add(std::make_shared<Quad>(glm::vec3(-2.f, 3.f, 1.f), glm::vec3(4.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 4.f), upper_orange));
    world.add(std::make_shared<Quad>(glm::vec3(-2.f, -3.f, 5.f), glm::vec3(4.f, 0.f, 0.f), glm::vec3(0.f, 0.f, -4.f), lower_teal));

    return world;
}

Camera testQuadSceneCamera(const glm::ivec2& imageSize) {
    glm::vec3 lookFrom(0.f, 0.f, 9.f);
    glm::vec3 lookAt(0.f, 0.f, 0.f);
    Camera::Projection projection(imageSize, 80.f, 1.f);
    Camera::Frame frame(lookFrom, lookAt);
    return Camera(frame, projection);
}

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
    glm::ivec2 imageSize = glm::ivec2(300, 300);
    HittableList world = cornellBoxScene();
    Camera camera = cornellBoxSceneCamera(imageSize);
    Image img(imageSize.x, imageSize.y);

    HittableList bvhWorld;
    bvhWorld.add(std::make_shared<BVHNode>(world.objects(), 0, world.objects().size()));
	
	Renderer renderer;
	renderer.render(bvhWorld, camera, img);

	img.gammaCorrect();

	img.write("C:\\Users\\markf\\GitHub\\raytracer\\render\\test.png");
}