#include "Mesh.h"

#include <GL/gl3w.h>

Mesh* Mesh::GenerateCubeMesh()
{
	float cubeVertices[] = {
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	};

	unsigned int cubeTriangles[] = {
		1,2,6,	6,5,1,
		0,4,7,	7,3,0,
		4,5,6,	6,7,4,
		0,3,2,	2,1,0,
		0,1,5,	5,4,0,
		3,7,6,	6,2,3
	};

	Mesh* cubeMesh = new Mesh();
	glGenVertexArrays(1, &cubeMesh->vao);
	glBindVertexArray(cubeMesh->vao);

	glGenBuffers(1, &cubeMesh->ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeMesh->ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 36, cubeTriangles, GL_STATIC_DRAW);

	glGenBuffers(1, &cubeMesh->vbo[0]); 
	glBindBuffer(GL_ARRAY_BUFFER, cubeMesh->vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 24, cubeVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	return cubeMesh;
}

Mesh* Mesh::GenerateSphereMesh()
{
	return nullptr;
}

Mesh* Mesh::GenerateLineMesh(const std::vector<glm::vec3>& points)
{
	Mesh* lineMesh = new Mesh();
	glGenVertexArrays(1, &lineMesh->vao);
	glBindVertexArray(lineMesh->vao);

	glGenBuffers(1, &lineMesh->vbo[0]);
	glBindBuffer(GL_ARRAY_BUFFER, lineMesh->vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * points.size(), points.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	return lineMesh;
}
