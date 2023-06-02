#include "GameObject.h"

#include<glm/gtc/quaternion.hpp>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/common.hpp>

#include <gl/GL.h>

GameObject::GameObject(
	const std::string& name,
	Transform* transform
) : name(name),
	transform(transform),
	activeInHierarchy(true)
{
	transform->gameObject = this;
#ifndef PROD
	transformGizmos = new TransformGizmos();
#endif
}

GameObject::GameObject(
	const std::string& name,
	const glm::vec3& position,
	const glm::quat& rotation,
	const glm::vec3 & scale
) :
	name(name),
	activeInHierarchy(true)
{
	transform = new Transform(position, rotation, scale);
	transform->gameObject = this;

#ifndef PROD
	transformGizmos = new TransformGizmos();
#endif
}


void GameObject::Update(float deltaTime)
{
	for (auto component : m_addedComponents) {
		if (!component->enabled) continue;
		m_components.push_back(component);
		component->Start();
	}
	m_addedComponents.clear();

	for (auto component : m_components) {
		if (!component->enabled) continue;
		component->Update(deltaTime);
	}

#ifndef PROD
	transformGizmos->Draw(transform->GetModel());
#endif
}

void GameObject::SetActive(bool state)
{
	activeInHierarchy = state;
}

void GameObject::ApplyOnHierarchy(std::function<void(GameObject*)> callback)
{
	callback(this);
	for (auto& child : transform->Childs()) {
		child->gameObject->ApplyOnHierarchy(callback);
	}
}
