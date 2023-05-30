#pragma once

#include <map>
#include <string>

#include "Data/Texture.h"
#include "Data/Mesh.h"
#include "Data/Material.h"
#include "Data/Shader.h"
#include "../GameObject.h"

class Ressources
{
private:
	std::map<std::string, Texture*> m_textures;
	std::map<std::string, Material*> m_materials;
	std::map<std::string, Mesh*> m_meshes;
	std::map<std::string, Shader*> m_shaders;
	std::map<std::string, GameObject*> m_prefabs;

public:
	Texture* GetTexture(const std::string& name) const;
	Material* GetMaterial(const std::string& name) const;
	Mesh* GetMesh(const std::string& name) const;
	Shader* GetShader(const std::string& name) const;
	GameObject* GetPrefab(const std::string& name) const;

	void AddTexture(const std::string& name, Texture* texture);
	void AddMaterial(const std::string& name, Material* material);
	void AddMesh(const std::string& name, Mesh* mesh);
	void AddPrefab(const std::string& name, GameObject* prefab);

	//Ressources& LoadMaterial(const std::string& filename);
	//Ressources& LoadMesh(const std::string& filename);

	Texture* LoadTextureFromFile(const std::string& filename);
	Texture* LoadTextureFromMemory(const std::string& name, const unsigned char* data, float width, float height);
	GameObject* LoadPrefab(const std::string& filename);
};

/*

            MeshRenderer
     Mesh <---'      '---> Material
                                '---> Texture

*/