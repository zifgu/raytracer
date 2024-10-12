#pragma once

#include "common.h"
#include "hittable_list.h"
#include "camera.h"
#include "image.h"
#include "material.h"

class Renderer {
	glm::vec3 envColor(const Ray& ray);
	glm::vec3 rayColor(const Hittable& world, const Ray& ray, int depth);
public:
	int samplesPerPixel = 500;
	int maxBounces = 50;

	void render(const Hittable& world, const Camera& camera, Image& output);
};