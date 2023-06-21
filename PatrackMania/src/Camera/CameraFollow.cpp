#include "CameraFollow.h"

void CameraFollow::Start()
{

}

void CameraFollow::Update(float deltaTime)
{
	transform->SetPosition(target->Position() + glm::vec3(0, 10, 0) + target->Up() * 40.f);
	//transform->SetPosition(target->Position() + (target->Forward() * -5.f) + glm::vec3(0, 0, 4));
	transform->SetForward(-target->Up());
}