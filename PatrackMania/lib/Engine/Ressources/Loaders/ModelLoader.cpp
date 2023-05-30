
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <tiny_gltf.h>

#include <GL/gl3w.h>

#include "ModelLoader.h"

#include "../Data/Mesh.h"
#include "../Data/Texture.h"
#include "../Data/Material.h"

#include "lib/Engine/Engine.h"

#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>

GameObject* ModelLoader::LoadFromFile(const std::string& filepath)
{
	tinygltf::Model model;
	tinygltf::TinyGLTF loader;
	std::string err;
	std::string warn;

	if (
		(filepath.ends_with(".glb") && !loader.LoadBinaryFromFile(&model, &err, &warn, filepath))
		|| !loader.LoadASCIIFromFile(&model, &err, &warn, filepath)
	) {
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
		tr->SetLocalPosition(translation);
		tr->SetLocalScale(scale);
		tr->SetLocalRotation(rotation);
	}

	for (auto& child : node.children) {
		LoadModelRec(model, model.nodes[child], go);
	}
	if (node.mesh == -1) return go;

	auto meshRenderer = go->AddComponent<MeshRenderer>();

	Mesh* mesh = new Mesh();
	auto& gltfMesh = model.meshes[node.mesh];
	for (auto p : gltfMesh.primitives) {
		for (auto [key, value] : p.attributes) {
			Console.Log(key);
		}
	}
	auto primitive = gltfMesh.primitives[0];


	const tinygltf::Accessor& accessorIndices = model.accessors[primitive.indices];
	const tinygltf::BufferView& bufferViewIndices = model.bufferViews[accessorIndices.bufferView];
	const tinygltf::Buffer& bufferIndices = model.buffers[bufferViewIndices.buffer];
	const unsigned int* indices = reinterpret_cast<const unsigned int*>(&bufferIndices.data[bufferViewIndices.byteOffset + accessorIndices.byteOffset]);

	const tinygltf::Accessor& accessorPosition = model.accessors[primitive.attributes["POSITION"]];
	const tinygltf::BufferView& bufferViewPosition = model.bufferViews[accessorPosition.bufferView];
	const tinygltf::Buffer& bufferPosition = model.buffers[bufferViewPosition.buffer];
	const float* positions = reinterpret_cast<const float*>(&bufferPosition.data[bufferViewPosition.byteOffset + accessorPosition.byteOffset]);

	const tinygltf::Accessor& accessorNormal = model.accessors[primitive.attributes["NORMAL"]];
	const tinygltf::BufferView& bufferViewNormal = model.bufferViews[accessorNormal.bufferView];
	const tinygltf::Buffer& bufferNormal = model.buffers[bufferViewNormal.buffer];
	const float* normals = reinterpret_cast<const float*>(&bufferNormal.data[bufferViewNormal.byteOffset + accessorNormal.byteOffset]);
	
	Console.Log(accessorIndices.count, accessorNormal.count, accessorPosition.count);

	//for (int i = 0; i < accessorPosition.count; ++i)
	//{
	//	Console.Log(positions[i * 3 + 0], positions[i * 3 + 1], positions[i * 3 + 2]);
	//}
	glGenVertexArrays(1, &mesh->vao);
	glBindVertexArray(mesh->vao);

	glGenBuffers(1, &mesh->ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * accessorIndices.count, indices, GL_STATIC_DRAW);

	glGenBuffers(1, &mesh->vbo[0]);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * accessorPosition.count, positions, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &mesh->vbo[1]);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * accessorNormal.count, normals, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	std::vector<float> uvs;
	Material* material = nullptr;
	if (primitive.attributes.find("TEXCOORD_0") != primitive.attributes.end()) {
		const tinygltf::Accessor& accessorTexture = model.accessors[primitive.attributes["TEXCOORD_0"]];
		const tinygltf::BufferView& bufferViewTexture = model.bufferViews[accessorTexture.bufferView];
		const tinygltf::Buffer& bufferTexture = model.buffers[bufferViewTexture.buffer];
		const float* textures = reinterpret_cast<const float*>(&bufferTexture.data[bufferViewTexture.byteOffset + accessorTexture.byteOffset]);

		Console.Log("texture", accessorTexture.count);
		uvs.assign(textures, textures + accessorTexture.count * 2);

		if (primitive.material >= 0)
		{
			auto& m = model.materials[primitive.material];
			material = new Material();
			*material = Material{
				.BaseColorTexture = nullptr,
				.BaseColor = Material::Color(
					m.pbrMetallicRoughness.baseColorFactor[0],
					m.pbrMetallicRoughness.baseColorFactor[1],
					m.pbrMetallicRoughness.baseColorFactor[2],
					m.pbrMetallicRoughness.baseColorFactor[3]
				),
				.Metallic = static_cast<float>(m.pbrMetallicRoughness.metallicFactor),
				.Roughness = static_cast<float>(m.pbrMetallicRoughness.roughnessFactor)
			};
			if (m.pbrMetallicRoughness.baseColorTexture.index != -1) {
				auto& t = model.textures[m.pbrMetallicRoughness.baseColorTexture.index];
				auto& img = model.images[t.source];
				Console.Log(img.width, img.height);
				auto texture = Engine::Ressources->LoadTextureFromMemory(img.uri, img.image.data(), img.width, img.height);
				material->BaseColorTexture = texture;
			}
			else {
				auto texture = Engine::Ressources->GetTexture("assets/textures/baseTexture.png");
				material->BaseColorTexture = texture;
			}
			Engine::Ressources->AddMaterial(m.name, material);
		}
	}
	else {
		uvs.resize(accessorPosition.count * 2);
		material = Engine::Ressources->GetMaterial("default");
	}
	glGenBuffers(1, &mesh->vbo[2]);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * uvs.size(), uvs.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	Engine::Ressources->AddMesh(node.name, mesh);
	meshRenderer->mesh = mesh;
	meshRenderer->material = material;
	return go;
}