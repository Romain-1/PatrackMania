#pragma once

#include <glm/matrix.hpp>

#include "lib/Engine/Ressources/Data/Mesh.h"
#include "lib/Engine/Ressources/Data/Shader.h"

class TransformGizmos
{
private:
	static inline const std::string vertexShader = 
		"#version 400\n"
		"layout(location = 0) in vec3 VertexPosition;\n"
		"out vec3 f_color;\n"
		"uniform vec3 color;\n"
		"uniform mat4 MVP;\n"
		"\n"
		"void main(void)\n"
		"{\n"
			"gl_Position = MVP * vec4(VertexPosition, 1.0f);\n"
			"f_color = color;\n"
		"}\n";

	static inline const std::string fragShader = 
		"#version 440"
		"\n"
		"in vec3 f_color;"
		"\n"
		"out vec4 FragColor;"
		"\n"
		"void main()\n"
		"{\n"
			"FragColor = vec4(f_color, 1.0);\n"
		"}\n";

	Mesh* forward;
	Mesh* up;
	Mesh* right;

	static inline Shader* m_shader = nullptr;

public:
	TransformGizmos();

	void Draw(const glm::mat4 &model) const;
};
