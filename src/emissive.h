#pragma once

#include "material.h"
#include "texture.h"

class DiffuseEmissive : public Material {
	std::shared_ptr<Texture> m_texture = nullptr;
public:
	DiffuseEmissive(glm::vec3 emit) : m_texture(std::make_shared<SolidColorTexture>(emit)) {}
	DiffuseEmissive(std::shared_ptr<Texture> texture) : m_texture(texture) {}

	virtual const glm::vec3& emitted(const glm::vec2& uv, const glm::vec3& p) const {
		return m_texture->value(uv, p);
	}
};