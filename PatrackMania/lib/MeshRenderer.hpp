#pragma once

#include "VertexAttr.hpp"

#include "VertexHandler.hpp"

#include "Loader.h"

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include <vector>
#include <array>
#include <string>

namespace tinygltf { class Model; class Node; }

class MeshRenderer
{
protected:
	VertexHandlers m_handler;
	int m_drawMode;
	GLint useTexture;

public:
	virtual void Draw(ShaderProgram* shader);
	MeshRenderer(
		const tinygltf::Model& model,
		const tinygltf::Node& node,
		int drawMode = GL_TRIANGLES
	);

	void ToggleTexture(bool state);
};