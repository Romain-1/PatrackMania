#include "CameraFollow.h"

void CameraFollow::Start()
{

}

void CameraFollow::Update(float deltaTime)
{
	
	transform->SetPosition(target->Position() + (target->Forward() * -5.f) + glm::vec3(0, 0, 4));
	transform->SetForward(target->Forward());
}