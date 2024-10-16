#include "image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

Image::Image(std::string file) {
	int n;
	float* rawData = stbi_loadf(file.c_str(), &w, &h, &n, c);
	if (rawData == nullptr) {
		w = h = 0;
		return;
	}
	data.resize(w * h * c);
	for (int x = 0; x < w; ++x) {
		for (int y = 0; y < h; ++y) {
			int dataIdx = y * w * c + x * c;
			float r = rawData[dataIdx + 0];
			float g = rawData[dataIdx + 1];
			float b = rawData[dataIdx + 2];
			set(x, y, glm::vec3(r, g, b));
		}
	}
}

void Image::write(std::string file) const {
	uint8_t* data = new uint8_t[w * h * c];
	for (int x = 0; x < w; ++x) {
		for (int y = 0; y < h; ++y) {
			glm::vec3 pixel = glm::clamp(get(x, y) * 255.0f, 0.f, 255.f);

			// stbi_write expects 8-bit values in left-to-right, top-to-bottom, RGB order
			int dataIdx = y * w * c + x * c;
			data[dataIdx + 0] = static_cast<uint8_t>(pixel.x);
			data[dataIdx + 1] = static_cast<uint8_t>(pixel.y);
			data[dataIdx + 2] = static_cast<uint8_t>(pixel.z);
		}
	}

	stbi_write_png(file.c_str(), w, h, c, data, w * c);

	delete[] data;
}

void Image::gammaCorrect(float gamma) {
	float inverseGamma = 1.f / gamma;

	for (int x = 0; x < w; ++x) {
		for (int y = 0; y < h; ++y) {
			glm::vec3 pixel = get(x, y);
			glm::vec3 corrected = glm::pow(glm::max(pixel, 0.f), glm::vec3(inverseGamma));
			set(x, y, corrected);
		}
	}
}