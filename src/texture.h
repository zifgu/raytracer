#pragma once

#include "common.h"
#include "image.h"

class Texture {
public:
	virtual ~Texture() = default;

	virtual const glm::vec3& value(glm::vec2 uv, glm::vec3 p) const = 0;
};

class SolidColorTexture : public Texture {
public:
	glm::vec3 color = glm::vec3(0.f);

	SolidColorTexture(const glm::vec3& color) : color(color) {}

	const glm::vec3& value(glm::vec2 uv, glm::vec3 p) const override {
		return color;
	};
};

class CheckerTexture : public Texture {
	float invScale = 1.f;
	std::shared_ptr<Texture> even;
	std::shared_ptr<Texture> odd;
public:
	CheckerTexture(float scale, std::shared_ptr<Texture> even, std::shared_ptr<Texture> odd) : invScale(1.f / scale), even(even), odd(odd) {}

	const glm::vec3& value(glm::vec2 uv, glm::vec3 p) const override {
		int x = static_cast<int>(p.x * invScale);
		int y = static_cast<int>(p.y * invScale);
		int z = static_cast<int>(p.z * invScale);
		bool isEven = (x + y + z) % 2 == 0;

		return isEven ? even->value(uv, p) : odd->value(uv, p);
	};
};

class ImageTexture : public Texture {
	Image image;
public:
	static const glm::vec3 emptyColor;

	ImageTexture(std::string file) : image(file) {}

	const glm::vec3& value(glm::vec2 uv, glm::vec3 p) const override {
		if (image.width() <= 0 || image.height() <= 0) return emptyColor;

		// nearest filtering
		float u = glm::clamp(uv.x, 0.f, 1.f);
		float v = 1.f - glm::clamp(uv.y, 0.f, 1.f);

		int x = static_cast<int>(u * image.width());
		int y = static_cast<int>(v * image.height());
		return image.get(x, y);
	};
};

const glm::vec3 ImageTexture::emptyColor = glm::vec3(1.f, 0.f, 1.f);