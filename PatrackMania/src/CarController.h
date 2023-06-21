#pragma once

#include "lib/Engine/MonoBehaviour.hpp"

class CarController : public MonoBehaviour
{
private:
	Transform* FL = nullptr;
	Transform* FR = nullptr;
	Transform* BL = nullptr;
	Transform* BR = nullptr;
	Transform* test = nullptr;
	float speed = 0.f;
	float acceleration = 10.f;

	void ControlWheel(float t, float deltaTime);

public:

	void Start() override;
	void Update(float deltaTime) override;
};
