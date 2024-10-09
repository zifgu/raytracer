#include "image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

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