
#include "TransformGizmos.h"

#include "lib/Engine/Engine.h"
#include <glm/gtc/type_ptr.hpp>

TransformGizmos::TransformGizmos()
{
	forward = Mesh::GenerateLineMesh({ glm::vec3(0, 0, 0), glm::vec3(0, 0, 100) });
	up = Mesh::GenerateLineMesh({ glm::vec3(0, 0, 0), glm::vec3(0, 100, 0) });
	right = Mesh::GenerateLineMesh({ glm::vec3(0, 0, 0), glm::vec3(100, 0, 0) });
	
	m_shader = new Shader();
	m_shader->initFromStrings(vertexShader, fragShader);
	m_shader->addUniform("color");
	m_shader->addUniform("projection");
	m_shader->addUniform("view");

}

void TransformGizmos::Draw() const
{
	const auto& projection = Engine::Graphics->GetCameraProjection();
	const auto& view = Engine::Graphics->GetCameraView();

	m_shader->use();

	glUniformMatrix4fv(m_shader->uniform("projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(m_shader->uniform("view"), 1, GL_FALSE, glm::value_ptr(view));

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
