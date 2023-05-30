#pragma once

#include <glm/vec4.hpp>
#include "Texture.h"

struct Material
{
	using Color = glm::vec4;

	const Texture* BaseColorTexture;
	Color BaseColor = Color(1, 1, 1, 1);
	float Metallic = 0.f;
	float Roughness = 1.f;
};

