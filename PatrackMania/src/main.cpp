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

const int WIDTH = 1280;
const int HEIGHT = 720;

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


void GenerateCubeMesh()
{
	float cubeVertices[] = {
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	};

	unsigned int cubeTriangles[] = {
		1,2,6,	6,5,1,
		0,4,7,	7,3,0,
		4,5,6,	6,7,4,
		0,3,2,	2,1,0,
		0,1,5,	5,4,0,
		3,7,6,	6,2,3
	};

	Mesh* cubeMesh = new Mesh();
	glGenVertexArrays(1, &cubeMesh->vao);
	glBindVertexArray(cubeMesh->vao);

	glGenBuffers(1, &cubeMesh->ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeMesh->ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 36, cubeTriangles, GL_STATIC_DRAW);

	glGenBuffers(1, &cubeMesh->vbo[0]);
	glBindBuffer(GL_ARRAY_BUFFER, cubeMesh->vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 24, cubeVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	Engine::Ressources->AddMesh("defaultCube", cubeMesh);
}

int main(int argc, char** argv)
{
	Engine::Ressources = new Ressources();
	Engine::Add(new Module::Graphics(WIDTH, HEIGHT));

	GenerateCubeMesh();

	Engine::Graphics->skybox = new SkyBox("assets/skyboxes/test");

	GameObject* road = ModelLoader::LoadFromFile("assets/models/track/scene.gltf");
	GameObject* car = ModelLoader::LoadFromFile("assets/models/car/car.gltf");

	car->AddComponent<CarController>();
	car->transform->Childs()[0]->SetLocalScale(glm::vec3(1, 1, 1));

	auto camera = Engine::Graphics->MainCamera()->gameObject;

	Engine::Add(car);
	Engine::Add(road);
	Engine::Add(camera);

	Engine::Run();
	return 0;
}