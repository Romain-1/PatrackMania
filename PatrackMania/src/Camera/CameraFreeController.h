#pragma once

#include "lib/Engine/MonoBehaviour.hpp"

class CameraFreeController : public MonoBehaviour
{
private:
	glm::vec2 m_mousePrevPosition;
	glm::vec2 m_currentRotation;

public:
	float sensitivity = 0.01f;
	float maxYAngle = 80.f;

public:
	void MouseDragging();
	void Update(float deltatime) override;
};

