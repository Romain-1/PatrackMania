
#include "TransformGizmos.h"

#include "lib/Engine/Engine.h"
#include <glm/gtc/type_ptr.hpp>

TransformGizmos::TransformGizmos()
{
	forward = Mesh::GenerateLineMesh({ glm::vec3(0, 0, 0), glm::vec3(0, 0, 100) });
	up = Mesh::GenerateLineMesh({ glm::vec3(0, 0, 0), glm::vec3(0, 100, 0) });
	right = Mesh::GenerateLineMesh({ glm::vec3(0, 0, 0), glm::vec3(100, 0, 0) });
	
	if (!m_shader)
	{
		m_shader = new Shader();
		m_shader->initFromStrings(vertexShader, fragShader);
		m_shader->addUniform("color");
		m_shader->addUniform("MVP");
	}

}

void TransformGizmos::Draw(const glm::mat4& model) const
{
	const auto& projection = Engine::Graphics->GetCameraProjection();
	const auto& view = Engine::Graphics->GetCameraView();
	const auto& mvp = model * view * projection;

	m_shader->use();

	glUniformMatrix4fv(m_shader->uniform("MVP"), 1, GL_FALSE, glm::value_ptr(mvp));

	{
		glUniformMatrix4fv(m_shader->uniform("color"), 1, GL_FALSE, glm::value_ptr(glm::vec3(0, 0, 1)));
		glBindVertexArray(forward->vao);
		glDrawArrays(GL_LINES, 0, 2);
		glBindVertexArray(0);
	}
	{
		glUniformMatrix4fv(m_shader->uniform("color"), 1, GL_FALSE, glm::value_ptr(glm::vec3(0, 1, 0)));
		glBindVertexArray(up->vao);
		glDrawArrays(GL_LINES, 0, 2);
		glBindVertexArray(0);
	}
	{
		glUniformMatrix4fv(m_shader->uniform("color"), 1, GL_FALSE, glm::value_ptr(glm::vec3(1, 0, 0)));
		glBindVertexArray(right->vao);
		glDrawArrays(GL_LINES, 0, 2);
		glBindVertexArray(0);
	}
	m_shader->disable();
}
