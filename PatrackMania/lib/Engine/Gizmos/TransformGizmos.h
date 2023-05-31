#pragma once

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
		"uniform mat4 projection;\n"
		"uniform mat4 view;"
		"\n"
		"void main(void)\n"
		"{\n"
			"gl_Position = view * projection * vec4(VertexPosition, 1.0f);\n"
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

	Shader* m_shader;

public:
	TransformGizmos();

	void Draw() const;
};
