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
