#pragma once

#include "../MonoBehaviour.hpp"
#include <cyclone.h>

class RigidBody : public MonoBehaviour
{
public:
	cyclone::CollisionBox *boxCollider;
	cyclone::RigidBody *rigidBody;

public:
	RigidBody();
	void Update(float deltatime) override;

};

