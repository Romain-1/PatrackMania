#pragma once

#include <vector>
#include <glm/vec3.hpp>

#include <GL/glcorearb.h>

struct Mesh
{
public:
	GLuint vao;
	GLuint ibo;
	GLuint vbo[3];

	static Mesh* GenerateCubeMesh();
	static Mesh* GenerateSphereMesh();
	static Mesh* GenerateLineMesh(const std::vector<glm::vec3>& points);
};


