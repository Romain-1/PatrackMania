#pragma once

#include "IModule.h"
#include <cyclone.h>

namespace Module
{
	class Physics : public IModule
	{
	public:
		cyclone::ForceRegistry* forceRegistry;
		std::vector<cyclone::Contact> contacts;
		cyclone::ContactResolver* resolver;

	public:
		void Init() override;
		void Update(float deltaTime, const std::vector<GameObject*>& objects) override;
	};
}

