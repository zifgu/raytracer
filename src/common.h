#pragma once

#include <limits>
#include <random>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/common.hpp>
#include <glm/geometric.hpp>

const float infinity = std::numeric_limits<float>::infinity();

inline float random() {
	static std::uniform_real_distribution<float> distribution(0.0, 1.0);
	static std::mt19937 generator;
	return distribution(generator);
}

inline float random(float min, float max) {
	return min + (max - min) * random();
}

inline float randomNormal() {
	static std::normal_distribution<float> distribution(0.0, 1.0);
	static std::mt19937 generator;
	return distribution(generator);
}

inline glm::vec3 randomOnSphere() {
	return glm::normalize(glm::vec3(randomNormal(), randomNormal(), randomNormal()));
}

inline glm::vec3 randomOnHemisphere(glm::vec3 normal) {
	glm::vec3 vec = randomOnSphere();
	return glm::sign(glm::dot(vec, normal)) * vec;
}