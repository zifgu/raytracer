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
	glm::vec3 m_color = glm::vec3(0.f);

	SolidColorTexture(const glm::vec3& color) : m_color(color) {}

	const glm::vec3& value(glm::vec2 uv, glm::vec3 p) const override {
		return m_color;
	};
};

class CheckerTexture : public Texture {
	float m_inverseScale = 1.f;
	std::shared_ptr<Texture> m_evenTexture = nullptr;
	std::shared_ptr<Texture> m_oddTexture = nullptr;
public:
	CheckerTexture(float scale, std::shared_ptr<Texture> even, std::shared_ptr<Texture> odd) : m_inverseScale(1.f / scale), m_evenTexture(even), m_oddTexture(odd) {}

	const glm::vec3& value(glm::vec2 uv, glm::vec3 p) const override {
		int x = static_cast<int>(glm::floor(p.x * m_inverseScale));
		int y = static_cast<int>(glm::floor(p.y * m_inverseScale));
		int z = static_cast<int>(glm::floor(p.z * m_inverseScale));
		bool isEven = (x + y + z) % 2 == 0;

		return isEven ? m_evenTexture->value(uv, p) : m_oddTexture->value(uv, p);
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

		int x = static_cast<int>(glm::floor(u * image.width()));
		int y = static_cast<int>(glm::floor(v * image.height()));
		return image.get(x, y);
	};
};
