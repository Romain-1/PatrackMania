#pragma once

#include "lib/Engine/MonoBehaviour.hpp"

class CameraFollow : public MonoBehaviour
{
public:
	glm::vec3 distanceFromObject;
	Transform* target;

public:
	void Start() override;
	void Update(float deltaTime) override;
};
