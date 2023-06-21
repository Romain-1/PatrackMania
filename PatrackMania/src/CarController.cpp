

//#include "lib/Engine/Engine.h"

#include "lib/Engine/GameObject.h"
#include "CarController.h"

#include <glm/gtx/compatibility.hpp>
#include <glm/gtx/string_cast.hpp>


void CarController::Start()
{
	FL = transform->Find("1FLWheelMe_driverwheels.001_0");
	FR = transform->Find("1FRWheelMe_driverwheels.001_0");
	test = transform->Find("1FLWheelMe");
}

void CarController::ControlWheel(float t, float deltaTime)
{
	FL->SetLocalRotation(
		glm::lerp(FL->LocalRotation(), glm::quat(glm::vec3(0.f, t / 3, 0.f)), 0.3f)
	);
	FR->SetLocalRotation(
		glm::lerp(FR->LocalRotation(), glm::quat(glm::vec3(0.f, t / 3, 0.f)), 0.3f)
	);

	transform->Rotate(glm::vec3(0, 1, 0), 90.f * deltaTime * t);
}

void CarController::Update(float deltaTime)
{
	if (Keyboard::GetKeyState(GLFW_KEY_A) == PRESSED) {
		ControlWheel(1, deltaTime);
	}
	else if (Keyboard::GetKeyState(GLFW_KEY_D) == PRESSED) {
		ControlWheel(-1, deltaTime);
	}
	else {
		//ControlWheel(0, deltaTime);
	}
	if (Keyboard::GetKeyState(GLFW_KEY_W) == PRESSED) {
		//transform->SetForward(glm::lerp(transform->Forward(), FL->Forward(), 0.3f));
		speed += deltaTime * acceleration;
		//Console.Log(glm::to_string(Engine::Graphics->MainCamera()->transform->Position()));
	}
	else {
		speed = (speed < 0) ? 0 : (speed - deltaTime * acceleration * 5);
	}
	if (speed >= 0)
	{
		//FL->SetLocalRotation();
		transform->SetPosition(transform->Position() + transform->Up() * -speed * (float)deltaTime);
	}

}