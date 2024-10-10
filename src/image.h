#pragma once

#include <string>
#include <cstdint>
#include <vector>

#include "common.h"

class Image {
private:
	int w = 0;
	int h = 0;
	const int c = 3;
	std::vector<glm::vec3> data;

	int idx(int x, int y) const {
		return y * w * c + x * c;
	}
public:
	Image(int width, int height) : w(width), h(height) {
		data.resize(w * h * c);
	}
	int width() const { return w; }
	int height() const { return h; }
	const glm::vec3& get(int x, int y) const {
		return data[idx(x, y)];
	}
	glm::vec3& get(int x, int y) {
		return data[idx(x, y)];
	}
	void set(int x, int y, const glm::vec3& val) {
		data[idx(x, y)] = val;
	}
	void gammaCorrect(float gamma = 2.2f);
	void write(std::string file) const;
};