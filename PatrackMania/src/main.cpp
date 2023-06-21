#include <GL/gl3w.h>

#include "lib/Engine/Engine.h"

#include "lib/Viewer.h"
#include "lib/Engine/Ressources/Loaders/ModelLoader.h"

#include <cyclone.h>

#include "lib/Console.hpp"
#include "lib/Engine/BaseComponents/MeshRenderer.hpp"
#include "lib/Engine/Input.h"
#include "lib/Engine/Ressources/Data/Shader.h"
#include "lib/Engine/GameObject.h"

#include <glm/gtx/string_cast.hpp>

#include "CarController.h"
#include "Camera/CameraFollow.h"
#include "Camera/CameraFreeController.h"

const int WIDTH = 1280;
const int HEIGHT = 720;

//std::vector<cyclone::RigidBody*> _rigidBodies;
//std::vector<cyclone::CollisionBox*> _collisionsBoxes;
//std::vector<cyclone::CollisionSphere*> _collisionsSpheres;
//std::vector<cyclone::Joint*> _joints;
//std::vector<cyclone::Spring*> _springs;
//std::vector<cyclone::Contact*> _contacts;
//cyclone::ContactResolver* _resolver;
//cyclone::ForceRegistry* _forceRegistry;


void GenerateCubeMesh()
{

}

int main(int argc, char** argv)
{
	Engine::Ressources = new Ressources();

	Engine::Add(new Module::Graphics(WIDTH, HEIGHT));

	Engine::Ressources->InitDefault();

	GenerateCubeMesh();

	Engine::Graphics->skybox = new SkyBox("assets/skyboxes/test");

	GameObject* road = ModelLoader::LoadFromFile("assets/models/track/scene.gltf");
	GameObject* car = ModelLoader::LoadFromFile("assets/models/car/car.gltf");

	car->AddComponent<CarController>();
	car->transform->Childs()[0]->SetLocalScale(glm::vec3(1, 1, 1));
	car->transform->SetPosition(glm::vec3(70, 3, 35));

	auto camera = Engine::Graphics->MainCamera()->gameObject;
	camera->transform->SetPosition(glm::vec3(100, 40, -80));
	camera->transform->SetForward(car->transform->Position() - camera->transform->Position());

	//GameObject* cube = new GameObject("cube", glm::vec3(0, 0, 0));
	//cube->transform->SetParent(car->transform);
	//cube->transform->SetLocalPosition(glm::vec3(0, -2, 2));
	//auto cubeMeshRenderer = cube->AddComponent<MeshRenderer>();
	//cubeMeshRenderer->mesh = Engine::Ressources->GetMesh("defaultCube");
	//cubeMeshRenderer->material = Engine::Ressources->GetMaterial("default");

	auto cameraFollow = camera->AddComponent<CameraFollow>();
	cameraFollow->target = car->transform;

	//Engine::Add(cube);
	Engine::Add(car);
	Engine::Add(road);
	Engine::Add(camera);


	Engine::Run();
	return 0;
}