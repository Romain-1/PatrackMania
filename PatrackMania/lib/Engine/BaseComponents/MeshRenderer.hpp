#pragma once

#include <GL/gl3w.h>

#include "lib/VertexAttr.hpp"
#include "lib/Engine/MonoBehaviour.hpp"

#include "lib/VertexHandler.hpp"

#include "lib/ShaderProgram.h"

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
	void Draw();
	void SetModel(
		const tinygltf::Model& model,
		const tinygltf::Node& node,
		int drawMode = GL_TRIANGLES
	);
};