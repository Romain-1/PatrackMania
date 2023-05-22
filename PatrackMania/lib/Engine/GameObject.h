#pragma once

#include "BaseComponents/Transform.h"
#include "BaseComponents/MeshRenderer.hpp"
#include "MonoBehaviour.hpp"

#include <functional>

class GameObject
{
private:
	std::vector<MonoBehaviour*> m_addedComponents;
	std::vector<MonoBehaviour*> m_components;

public:
	bool activeInHierarchy;
	std::string name;

	Transform* transform;


	GameObject(
		const std::string &name,
		Transform* transform
	);

	void Update(float deltaTime);

	void ApplyOnHierarchy(std::function<void(GameObject *)> callback);
	void SetActive(bool state);

	template<typename T> T* GetComponent() const
	{
		for (auto component : m_components) {
			T* r = dynamic_cast<T*>(component);
			if (r != nullptr) {
				return r;
			}
		}
		return nullptr;
	}

	template<typename T> T* AddComponent()
	{
		T* component = new T();
		MonoBehaviour* r = dynamic_cast<MonoBehaviour*>(component);

		if (r != nullptr) {
			m_addedComponents.push_back(r);
			r->gameObject = this;
			r->transform = transform;
			r->enabled = true;
		} else {
			throw new std::runtime_error("Trying to add a component that does not inherit from MonoBehaviour.");
		}
		return component;
	}

};

