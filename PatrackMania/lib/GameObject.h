#pragma once
#include "Transform.h"
#include "MeshRenderer.hpp"
#include "Loader.h"
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
	MeshRenderer* meshRenderer;
	ShaderProgram* shaderProgram;


	GameObject(
		const std::string &name,
		Transform* transform,
		MeshRenderer* meshRenderer = nullptr,
		ShaderProgram* shaderProgram = nullptr
	);

	void SetShader(ShaderProgram* shaderProgram);

	void Update(float deltaTime);
	void Draw(
		const glm::mat4& view,
		const glm::mat4& projection,
		const glm::mat4& parentModel = glm::mat4(1.f),
		int indent = 0
	) const;

	void ApplyOnHierarchy(std::function<void(GameObject *)> callback);
	void SetActive(bool state);

	template<typename T> T* GetComponent() const;
	template<typename T> void AddComponent() {
		T* component = new T();
		MonoBehaviour* r = dynamic_cast<MonoBehaviour*>(component);

		if (r != nullptr) {
			m_addedComponents.push_back(r);
			r->gameObject = this;
			r->transform = transform;
			r->enabled = true;
		}
		else {
			throw new std::runtime_error("Trying to add a component that does not inherit from MonoBehaviour.");
		}
	}

};

