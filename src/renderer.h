#pragma once

#include "common.h"
#include "hittable_list.h"
#include "projection.h"
#include "image.h"
#include "material.h"

class Renderer {
	glm::vec3 envColor(const Ray& ray);
	glm::vec3 rayColor(const Hittable& world, const Ray& ray, int depth);
public:
	int samplesPerPixel = 100;
	int maxBounces = 10;

	void render(const Hittable& world, const Projection& projection, Image& output);
};