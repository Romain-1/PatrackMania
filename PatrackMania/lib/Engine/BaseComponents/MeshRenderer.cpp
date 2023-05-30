#include <GL/gl3w.h>

#include "MeshRenderer.hpp"
#include "lib/Engine/Engine.h"
#include "lib/Engine/Ressources/Data/Shader.h"

#include <glm/gtc/type_ptr.hpp>

const glm::vec4 lightPos(10, 5, 10, 1);
const glm::vec3 La(1, 1, 1);
const glm::vec3 Ld(1, 1, 1);
const glm::vec3 Ls(1, 1, 1);

const GLfloat shiness = 0.4;
const glm::vec3 Ka(1, 1, 1);
const glm::vec3 Kd(0.9, 0.9, 0.9);
const glm::vec3 Ks(0.1, 0.1, 0.1);

void MeshRenderer::Draw()
{
	auto shader = Engine::Graphics->GetShader();
	auto view = Engine::Graphics->GetCameraView();

	auto model = transform->GetModel();
	glm::mat4 mview = view * model;

	glm::mat4 imvp = glm::inverse(mview);
	glm::mat3 nmat = glm::mat3(glm::transpose(imvp));
	glm::mat4 mvp = Engine::Graphics->GetCameraProjection() * view * model;

	shader->use();

	glUniform4fv(shader->uniform("Light.Position"), 1, glm::value_ptr(lightPos));
	glUniform3fv(shader->uniform("Light.La"), 1, glm::value_ptr(La));
	glUniform3fv(shader->uniform("Light.Ld"), 1, glm::value_ptr(Ld));
	glUniform3fv(shader->uniform("Light.Ls"), 1, glm::value_ptr(Ls));

	glUniformMatrix4fv(shader->uniform("ModelViewMatrix"), 1, GL_FALSE, glm::value_ptr(mview));
	glUniformMatrix3fv(shader->uniform("NormalMatrix"), 1, GL_FALSE, glm::value_ptr(nmat));
	glUniformMatrix4fv(shader->uniform("MVP"), 1, GL_FALSE, glm::value_ptr(mvp));

	glUniform3fv(shader->uniform("Material.Ka"), 1, glm::value_ptr(glm::vec3(material->BaseColor)));
	glUniform3fv(shader->uniform("Material.Kd"), 1, glm::value_ptr(glm::vec3(0.9, 0.9, 0.9)));
	glUniform3fv(shader->uniform("Material.Ks"), 1, glm::value_ptr(glm::vec3(0.1, 0.1, 0.1)));
	glUniform1fv(shader->uniform("Material.Shiness"), 1, &shiness);

	glBindTexture(GL_TEXTURE_2D, *material->BaseColorTexture);
	glBindVertexArray(mesh->vao);
	int size;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	glDrawElements(GL_TRIANGLES, size / sizeof(unsigned int), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
