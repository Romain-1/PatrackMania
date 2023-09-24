#include "RigidBody.h"

RigidBody::RigidBody() :
	boxCollider(new cyclone::CollisionBox()),
	rigidBody(new cyclone::RigidBody())
{
	boxCollider->body = rigidBody;
}

cyclone::Vector3 glmToCyclone(const glm::vec3& p)
{
	return cyclone::Vector3(p.x, p.y, p.z);
}

void RigidBody::Update(float deltatime)
{
	rigidBody->setPosition(glmToCyclone(transform->Position()));
	rigidBody->setRotation(glmToCyclone(glm::eulerAngles(transform->Rotation())));

	rigidBody->integrate(deltatime);
	boxCollider->calculateInternals();
}