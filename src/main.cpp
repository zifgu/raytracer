#include <iostream>
#include <limits>

#include <glm/vec3.hpp>

#include "image.h"
#include "ray.h"
#include "sphere.h"
#include "hittable_list.h"
#include "projection.h"
#include "renderer.h"

int main() {
	HittableList world;
	world.add(std::make_shared<Sphere>(glm::vec3(0.f, 0.f, -1.f), 0.5f));
	
	glm::ivec2 imageSize = glm::ivec2(320, 180);
	Projection projection(imageSize, glm::vec2(3.55555556f, 2.0f), 1.f);

	Image img(imageSize.x, imageSize.y);
	
	Renderer renderer;
	renderer.render(world, projection, img);

	img.write("C:\\Users\\markf\\GitHub\\raytracer\\render\\test.png");
}