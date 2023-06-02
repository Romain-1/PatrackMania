#include "CameraFreeController.h"

void CameraFreeController::MouseDragging()
{
	float mouseX = Mouse::cursor.x - m_mousePrevPosition.x;
	float mouseY = Mouse::cursor.y - m_mousePrevPosition.y;

	m_currentRotation.x += mouseX * sensitivity;
	m_currentRotation.y -= mouseY * sensitivity;

	if (m_currentRotation.x >= 360) m_currentRotation -= 360.f;
	if (m_currentRotation.x < 0) m_currentRotation += 360.f;
	m_currentRotation.y = glm::clamp(m_currentRotation.y, -maxYAngle, maxYAngle);
	transform->SetRotation(glm::quat(glm::vec3(m_currentRotation.y, m_currentRotation.x, 0)));

	m_mousePrevPosition = Mouse::cursor;
}

void CameraFreeController::Update(float deltaTime)
{
	if (Mouse::Any(JUST_PRESSED))
		m_mousePrevPosition = Mouse::cursor;
	if (Mouse::Any(PRESSED))
		MouseDragging();
	if (Keyboard::GetKeyState(GLFW_KEY_W))
		transform->SetPosition(transform->Position() + transform->Forward() * 10.f * deltaTime);
}