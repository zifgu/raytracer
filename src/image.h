#pragma once

#include <string>
#include <cstdint>
#include <vector>
#include <string>

#include "common.h"

class Image {
private:
	int m_width = 0;
	int m_height = 0;
	const int m_channels = 3;
	std::vector<glm::vec3> m_data;

	int idx(int x, int y) const {
		return y * m_width * m_channels + x * m_channels;
	}

	static glm::vec3 gammaCorrect(glm::vec3& pix, float gamma) {
		float inverseGamma = 1.f / gamma;
		return glm::pow(glm::max(pix, 0.f), glm::vec3(inverseGamma));
	}
public:
	// read image from file; will be transformed into linear space using provided gamma
	Image(std::string file, float gamma = 2.2f);
	Image(int width, int height) : m_width(width), m_height(height) {
		m_data.resize(m_width * m_height * m_channels);
	}
	int width() const { return m_width; }
	int height() const { return m_height; }
	const glm::vec3& get(int x, int y) const {
		return m_data[idx(x, y)];
	}
	glm::vec3& get(int x, int y) {
		return m_data[idx(x, y)];
	}
	void set(int x, int y, const glm::vec3& val) {
		m_data[idx(x, y)] = val;
	}
	// write image to file; will be encoded using the provided gamma value
	void write(std::string file, float gamma = 2.2f) const;
};