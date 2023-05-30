#pragma once

#include "lib/Engine/MonoBehaviour.hpp"
#include "lib/Engine/Ressources/Data/Mesh.h"
#include "lib/Engine/Ressources/Data/Material.h"

class MeshRenderer : public MonoBehaviour
{
public:
	Mesh* mesh;
	Material* material;

public:
	void Draw();
};