#pragma once

#include "../MonoBehaviour.hpp"

class Camera : public MonoBehaviour
{
public:
	float fov = 45.f; // in degrees
	float zNear = 0.1f;
	float zFar = 300.f;
};

