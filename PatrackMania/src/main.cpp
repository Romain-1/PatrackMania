#include "lib/Engine/Engine.h"

#include "lib/Viewer.h"
#include "lib/Engine/Loaders/ModelLoader.h"

#include <cyclone.h>

#include "lib/Console.hpp"
#include "lib/Engine/BaseComponents/MeshRenderer.hpp"
#include "lib/Engine/Input.h"
#include "lib/ShaderProgram.h"
#include "lib/Engine/GameObject.h"


#include "CarController.h"

const int WIDTH = 800;
const int HEIGHT = 600;

void MouseDragging(Viewer &viewer, glm::vec2& lastMouse)
{
	glm::vec2 fracChange(
		static_cast<float>(Mouse::cursor.x - lastMouse.x) / static_cast<float>(WIDTH),
		static_cast<float>(lastMouse.y - Mouse::cursor.y) / static_cast<float>(HEIGHT)
	);

	if (Mouse::GetButtonState(GLFW_MOUSE_BUTTON_1) == PRESSED) {
		viewer.rotate(fracChange.x, fracChange.y);
	}
	else if (Mouse::GetButtonState(GLFW_MOUSE_BUTTON_3) == PRESSED)
		viewer.zoom(fracChange.y);
	else if (Mouse::GetButtonState(GLFW_MOUSE_BUTTON_2) == PRESSED)
		viewer.translate(-fracChange.x, -fracChange.y, 1);
	lastMouse = Mouse::cursor;
}

//std::vector<cyclone::RigidBody*> _rigidBodies;
//std::vector<cyclone::CollisionBox*> _collisionsBoxes;
//std::vector<cyclone::CollisionSphere*> _collisionsSpheres;
//std::vector<cyclone::Joint*> _joints;
//std::vector<cyclone::Spring*> _springs;
//std::vector<cyclone::Contact*> _contacts;
//cyclone::ContactResolver* _resolver;
//cyclone::ForceRegistry* _forceRegistry;

int main(int argc, char** argv)
{
	Engine::Add(new Module::Graphics(WIDTH, HEIGHT));


	//GameObject* road = ModelLoader::LoadFromFile("assets/models/track/scene.gltf");
	GameObject* car = ModelLoader::LoadFromFile("assets/models/car/car.gltf");

	car->AddComponent<CarController>();
	car->transform->Childs()[0]->SetLocalScale(glm::vec3(1, 1, 1));

	Engine::Add(car);
	//Engine::Add(road);

	Engine::Run();
	return 0;
}