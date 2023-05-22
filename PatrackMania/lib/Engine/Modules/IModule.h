#pragma once

#include <vector>
#include "lib/Engine/GameObject.h"

class IModule
{
public:
	~IModule() = default;

	virtual void Init() = 0;
	virtual void Update(float deltaTime, const std::vector<GameObject *> &objects) = 0;
};