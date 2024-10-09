#pragma once

#include "hittable_list.h"
#include "projection.h"
#include "image.h"

class Renderer {
	glm::vec3 rayColor(const Hittable& world, const Ray& ray);
public:
	int samplesPerPixel = 10;

	void render(const Hittable& world, const Projection& projection, Image& output);
};