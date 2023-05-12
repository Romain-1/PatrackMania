#pragma once
#include "lib/MonoBehaviour.hpp"

class CarController : public MonoBehaviour
{
private:
	Transform* FL = nullptr;
	Transform* FR = nullptr;
	Transform* BL = nullptr;
	Transform* BR = nullptr;

	void ControlWheel(float t, float deltaTime);

public:

	void Start() override;
	void Update(float deltaTime) override;
};
