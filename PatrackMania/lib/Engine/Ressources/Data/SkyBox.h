#pragma once

#include "Shader.h"
#include "Texture.h"
#include "Mesh.h"

class SkyBox
{
private:
	Shader* m_shader;
	Texture* m_texture;
	Mesh* m_mesh;

public:
	SkyBox(const std::string &folder);

	void Draw();
};

