#include "Ressources.h"

#include "Loaders/ModelLoader.h"
#include <stb_image.h>
#include <GL/gl3w.h>

#include <iostream>

void Ressources::InitDefault()
{
	AddMesh("defaultCube", Mesh::GenerateCubeMesh());
	AddMesh("defaultSphere", Mesh::GenerateSphereMesh());
}

Texture* Ressources::GetTexture(const std::string& name) const { return m_textures.at(name); }
Material* Ressources::GetMaterial(const std::string& name) const { return m_materials.at(name); }
Mesh* Ressources::GetMesh(const std::string& name) const { return m_meshes.at(name); }
GameObject* Ressources::GetPrefab(const std::string& name) const { return m_prefabs.at(name); }

void Ressources::AddTexture(const std::string& filename, Texture* texture) { m_textures[filename] = texture; }
void Ressources::AddMaterial(const std::string& filename, Material* material) { m_materials[filename] = material; }
void Ressources::AddMesh(const std::string& filename, Mesh* mesh) { m_meshes[filename] = mesh; }
void Ressources::AddPrefab(const std::string& filename, GameObject* prefab) { m_prefabs[filename] = prefab; }

//Ressources& Ressources::LoadTexture(const std::string& filename)
//{
//	return *this;
//}
//
//Ressources& Ressources::LoadMaterial(const std::string& filename)
//{
//	return *this;
//}
//
//Ressources& Ressources::LoadMesh(const std::string& filename)
//{
//	throw new std::runtime_error("Error not implemented");
//}


//Ressources& LoadTexture(const std::string& name, const std::vector<unsigned char>& data)
//{
//	auto* id = new Texture();
//
//
//}

Texture* Ressources::LoadTextureFromFile(const std::string& filename)
{
	// maybe put this elsewhere
	if (m_textures.find(filename) != m_textures.end()) {
		return m_textures[filename];
	}
	int width, height, bpp;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &bpp, 4);

	if (data == nullptr) {
		Console.Log("Could not load", filename);
		return nullptr;
	}
	return LoadTextureFromMemory(filename, data, width, height);
}

Texture* Ressources::LoadTextureFromMemory(const std::string& name, const unsigned char * data, float width, float height)
{
	// maybe put this elsewhere
	if (m_textures.find(name) != m_textures.end()) {
		return m_textures[name];
	}
	auto texture = new Texture();

	glGenTextures(1, texture);
	glBindTexture(GL_TEXTURE_2D, *texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	AddTexture(name, texture);
	return texture;
}


GameObject* Ressources::LoadPrefab(const std::string& filename)
{
	GameObject* obj = ModelLoader::LoadFromFile(filename);

	AddPrefab(filename, obj);
	return obj;
}