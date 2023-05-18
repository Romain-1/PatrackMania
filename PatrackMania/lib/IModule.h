#pragma once

#include <vector>
#include "GameObject.h"

class IModule
{
public:
	virtual void Init() = 0;
	virtual void Update(float deltaTime, std::vector<GameObject *> objects) = 0;
};