#pragma once

#include "VertexAttr.hpp"
#include "MonoBehaviour.hpp"

#include "VertexHandler.hpp"

#include "ShaderProgram.h"

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include <vector>
#include <array>
#include <string>

namespace tinygltf { class Model; class Node; }

class MeshRenderer : public MonoBehaviour
{
private:
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