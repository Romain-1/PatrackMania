#pragma once

#include "BaseComponents/Transform.h"
#include "lib/Engine/Input.h"
#include "lib/Console.hpp"

class MonoBehaviour
{
public:
	bool enabled;
	GameObject* gameObject;
	Transform* transform;

	virtual void Start() {}
	virtual void Update(float deltaTime) {}
};