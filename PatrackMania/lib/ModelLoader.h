#pragma once

#include "GameObject.h"

namespace tinygltf { class Model; class Node; }

class ModelLoader
{
private:
	static GameObject* LoadModelRec(
		const tinygltf::Model& model,
		const tinygltf::Node& node,
		GameObject* parent
	);

public:
	static GameObject* LoadFromFile(const std::string& filepath);
};

