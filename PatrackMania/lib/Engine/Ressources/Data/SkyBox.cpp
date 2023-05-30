
#include <stb_image.h>

#include "SkyBox.h"
#include "lib/Console.hpp"

#include "lib/Engine/Engine.h"
#include <glm/gtc/type_ptr.hpp>

SkyBox::SkyBox(const std::string& folder)
{
	m_mesh = Engine::Ressources->GetMesh("defaultCube");
	m_texture = new Texture();

	glGenTextures(1, m_texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, *m_texture);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	std::string images[] = {
		folder + "/px.png",
		folder + "/nx.png",
		folder + "/py.png",
		folder + "/ny.png",
		folder + "/pz.png",
		folder + "/nz.png",
	};

	for (unsigned int i = 0; i < 6; ++i)
	{
		int width, height, channels;
		auto data = stbi_load(images[i].c_str(), &width, &height, &channels, 3);
		if (data)
		{
			stbi_set_flip_vertically_on_load(false);
			glTexImage2D(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0,
				GL_RGB,
				width, height,
				0,
				GL_RGB,
				GL_UNSIGNED_BYTE,
				data
			);
			stbi_image_free(data);
		}
		else
		{
			Console.Log("failed to load image", images[i]);
			stbi_image_free(data);
		}
	}

	m_shader = new Shader();
	m_shader->initFromFiles("assets/shaders/skybox.vert", "assets/shaders/skybox.frag");
	m_shader->addUniform("projection");
	m_shader->addUniform("view");
}

void SkyBox::Draw()
{
	const auto& projection = Engine::Graphics->GetCameraProjection();
	const auto& view = Engine::Graphics->GetCameraView();

	glDepthFunc(GL_LEQUAL);
	m_shader->use();

	glUniformMatrix4fv(m_shader->uniform("projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(m_shader->uniform("view"), 1, GL_FALSE, glm::value_ptr(view));

	glBindVertexArray(m_mesh->vao);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, *m_texture);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glDepthMask(GL_TRUE);
	glBindVertexArray(0);

	glDepthFunc(GL_LESS);
}