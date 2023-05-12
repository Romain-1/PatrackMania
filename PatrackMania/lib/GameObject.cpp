#include "GameObject.h"

#include<glm/gtc/quaternion.hpp>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/common.hpp>

#include <gl/GL.h>

GameObject::GameObject(
	const std::string& name,
	Transform* transform,
	MeshRenderer* meshRenderer,
	ShaderProgram* shaderProgram
) : name(name),
	transform(transform),
	meshRenderer(meshRenderer),
	shaderProgram(shaderProgram),
	activeInHierarchy(true)
{
	transform->gameObject = this;
}


void GameObject::Update(float deltaTime)
{
	for (auto component : m_addedComponents) {
		m_components.push_back(component);
		component->Start();
	}
	m_addedComponents.clear();

	for (auto component : m_components) {
		component->Update(deltaTime);
	}
}

void GameObject::Draw(const glm::mat4 &view, const glm::mat4 &projection, const glm::mat4 &parentModel, int indent) const
{
	glm::mat4 model = parentModel;
	model = glm::translate(model, transform->Position());
	model = glm::scale(model, transform->Scale());
	model = model * glm::toMat4(transform->Rotation());

	for (auto& child : transform->Childs()) {
		child->gameObject->Draw(view, projection, model, indent + 1);
	}

	//glLineWidth(3.0f);
	//glBegin(GL_LINES);
	//glColor3f(1, 0, 0);

	//glVertex3f(0.0f, 0.1f, 0.0f);
	//glVertex3f(0.0f, 100.0f, 0.0f);

	//glColor3f(0.0f, 11.0f, 0.0f);

	//glVertex3d(0.0f, 0.1f, 0.0f);
	//glVertex3d(100.0f, 0.1, 0.0f);

	//glColor3f(0.0f, 0.0f, 1.0f);

	//glVertex3f(0.0f, 0.1f, 0.0f);
	//glVertex3f(0.0f, 0.1f, 100.0f);
	//glEnd();
	//glLineWidth(1.0f);

	if (meshRenderer == nullptr) {
		return;
	}

	glm::mat4 mview = view * model;
	glm::mat4 imvp = glm::inverse(mview);
	glm::mat3 nmat = glm::mat3(glm::transpose(imvp));
	glm::mat4 mvp = projection * view * model;

	if (shaderProgram) {
		glUniformMatrix4fv(shaderProgram->uniform("ModelViewMatrix"), 1, GL_FALSE, glm::value_ptr(mview)); //View*Model : mat4
		glUniformMatrix3fv(shaderProgram->uniform("NormalMatrix"), 1, GL_FALSE, glm::value_ptr(nmat)); //Refer next slide : mat3
		glUniformMatrix4fv(shaderProgram->uniform("MVP"), 1, GL_FALSE, glm::value_ptr(mvp));
	}
	meshRenderer->Draw(shaderProgram);
}

void GameObject::SetShader(ShaderProgram* shaderProgram)
{
	this->shaderProgram = shaderProgram;

	for (auto& child : transform->Childs()) {
		child->gameObject->SetShader(shaderProgram);
	}
}

void GameObject::SetActive(bool state)
{
	activeInHierarchy = state;
}

void GameObject::ApplyOnHierarchy(std::function<void(GameObject*)> callback)
{
	callback(this);
	for (auto& child : transform->Childs()) {
		callback(child->gameObject);
		child->gameObject->ApplyOnHierarchy(callback);
	}
}

template<typename T>
T* GameObject::GetComponent() const
{
	for (auto component : m_components) {
		T* r = dynamic_cast<T*>(component);
		if (r != nullptr) {
			return r;
		}
	}
}

//template<typename T>
//void GameObject::AddComponent()
//{
//
//}