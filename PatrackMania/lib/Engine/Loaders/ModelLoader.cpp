
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <tiny_gltf.h>

#include "ModelLoader.h"


#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>

GameObject* ModelLoader::LoadFromFile(const std::string& filepath)
{
	tinygltf::Model model;
	tinygltf::TinyGLTF loader;
	std::string err;
	std::string warn;

	if (!loader.LoadASCIIFromFile(&model, &err, &warn, filepath)) {
		Console.Log(err, warn);
		return nullptr;
	}
	return LoadModelRec(model, model.nodes[0], nullptr);
}

GameObject* ModelLoader::LoadModelRec(
	const tinygltf::Model& model,
	const tinygltf::Node& node,
	GameObject* parent
)
{
	Console.Log(node.name, "child of", parent ? parent->name : "null");
	Transform* tr = new Transform();
	GameObject* go = new GameObject(node.name, tr);

	if (parent) tr->SetParent(parent->transform);
	if (node.translation.size() == 3)	tr->SetPosition(glm::vec3(node.translation[0], node.translation[1], node.translation[2]));
	if (node.scale.size() == 3)			tr->SetLocalScale(glm::vec3(node.scale[0], node.scale[1], node.scale[2]));
	if (node.rotation.size() == 4) {
		tr->SetRotation(glm::quat(
			static_cast<float>(node.rotation[0]), static_cast<float>(node.rotation[1]),
			static_cast<float>(node.rotation[2]), static_cast<float>(node.rotation[3])
		));
	}

	if (node.matrix.size() == 16) {
		glm::mat4 transformation = glm::make_mat4(node.matrix.data()); // your transformation matrix.
		glm::vec3 scale;
		glm::quat rotation;
		glm::vec3 translation;
		glm::vec3 skew;
		glm::vec4 perspective;
		glm::decompose(transformation, scale, rotation, translation, skew, perspective);
		Console.Log(__func__, __LINE__, glm::to_string(translation));
		tr->SetPosition(translation);
		Console.Log(__func__, __LINE__, glm::to_string(translation));
		tr->SetLocalScale(scale);
		tr->SetRotation(rotation);
	}

	for (auto& child : node.children) {
		LoadModelRec(model, model.nodes[child], go);
	}
	if (node.mesh == -1) return go;
	go->AddComponent<MeshRenderer>()->SetModel(model, node);
	return go;
}