#include "CarController.h"

#include <glm/gtx/compatibility.hpp>
#include <glm/gtx/string_cast.hpp>

void CarController::Start()
{
	FL = transform->Find("1FLWheelMe_driverwheels.001_0");
	FR = transform->Find("1FRWheelMe_driverwheels.001_0");
}

void CarController::ControlWheel(float t, float deltaTime)
{
	//FL->SetRotation(
	//	glm::lerp(FL->Rotation(), glm::quat(glm::vec3(0.f, t / 3, 0.f)), 0.3f)
	//);
	//FR->SetRotation(
	//	glm::lerp(FR->Rotation(), glm::quat(glm::vec3(0.f, t / 3, 0.f)), 0.3f)
	//);

	//transform->Rotate(0, 0, t * deltaTime);
}

void CarController::Update(float deltaTime)
{
	Console.Log("CarController::Update", glm::to_string(transform->Position()));
	if (Keyboard::GetKeyState(GLFW_KEY_A) == PRESSED) {
		ControlWheel(1, deltaTime);
	}
	else if (Keyboard::GetKeyState(GLFW_KEY_D) == PRESSED) {
		ControlWheel(-1, deltaTime);
	}
	else {
		ControlWheel(0, deltaTime);
	}
	if (Keyboard::GetKeyState(GLFW_KEY_W) == PRESSED) {
		//transform->SetForward(glm::lerp(transform->Forward(), FL->Forward(), 0.3f));
		transform->SetPosition(transform->Position() + transform->Forward() * -10.f * (float)deltaTime);
	}
}