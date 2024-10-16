#include "renderer.h"

#include <iostream>

void Renderer::render(const Hittable& world, const Camera& camera, Image& output) {
	const float sampleFrac = 1.f / static_cast<float>(m_samplesPerPixel);
	
	for (int y = 0; y < output.height(); ++y) {
		std::clog << "\rScanlines remaining: " << (output.height() - y) << ' ' << std::flush;

		for (int x = 0; x < output.width(); ++x) {
			glm::vec3 color = glm::vec3(0.f);
			for (int s = 0; s < m_samplesPerPixel; ++s) {
				Ray ray = camera.getRay(x, y, true);
				color += rayColor(world, ray, m_maxBounces);
			}
			output.set(x, y, color * sampleFrac);
		}
	}

	std::clog << "\rDone.                 \n";
}

glm::vec3 Renderer::rayColor(const Hittable& world, const Ray& ray, int depth) {
	if (depth < 0) return glm::vec3(0.f);

	float reflectance = 0.1f;
	const float eps = 1e-3f;
	Hittable::HitRecord hit;
	if (!world.hit(ray, Interval(eps, infinity), hit))
		return envColor(ray);

	glm::vec3 colorScattered = glm::vec3(0.f);
	Ray scatteredRay;
	glm::vec3 attenuation;
	if (hit.material->scatter(ray, hit, attenuation, scatteredRay)) {
		colorScattered = attenuation * rayColor(world, scatteredRay, depth - 1);
	}
	
	glm::vec3 colorEmitted = hit.material->emitted(hit.uv, hit.point);

	return colorEmitted + colorScattered;
}

glm::vec3 Renderer::envColor(const Ray& ray) {
	return glm::vec3(0.f);

	//float a = glm::normalize(ray.direction()).y * 0.5f + 0.5f;
	//return glm::mix(glm::vec3(1.0f), glm::vec3(0.5f, 0.7f, 1.0f), a);
}