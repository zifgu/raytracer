#include "renderer.h"

void Renderer::render(const Hittable& world, const Projection& projection, Image& output) {
	const float sampleFrac = 1.f / static_cast<float>(samplesPerPixel);
	
	for (int y = 0; y < output.height(); ++y) {
		for (int x = 0; x < output.width(); ++x) {
			glm::vec3 color = glm::vec3(0.f);
			for (int s = 0; s < samplesPerPixel; ++s) {
				Ray ray = projection.getRay(glm::ivec2(x, y), true);
				color += rayColor(world, ray);
			}
			output.set(x, y, color * sampleFrac);
		}
	}
}

glm::vec3 Renderer::rayColor(const Hittable& world, const Ray& ray) {
	Hittable::HitRecord hit;
	if (world.hit(ray, Interval::positive(), hit)) {
		return hit.normal * 0.5f + 0.5f;
	}
	else {
		return glm::vec3(1.f);
	}
}
