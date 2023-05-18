#include <stdint.h>
#include <iostream>
#include <chrono>


//#include <GL/glew.h>
#include "lib/Viewer.h"
#include "lib/ModelLoader.h"

#include <cyclone.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include "lib/Console.hpp"
#include "lib/MeshRenderer.hpp"
#include "lib/Input.h"
#include "lib/ShaderProgram.h"
#include "lib/GameObject.h"
//#include "lib/Texture.h"

#include "CarController.h"

const int WIDTH = 800;
const int HEIGHT = 600;

void MouseDragging(GLFWwindow *window, Viewer &viewer, glm::vec2& lastMouse)
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

void Draw(GLFWwindow *window, ShaderProgram *shaderProgram, std::vector<GameObject *> &gameObjects, Viewer &viewer)
{

}

std::vector<cyclone::RigidBody*> _rigidBodies;
std::vector<cyclone::CollisionBox*> _collisionsBoxes;
std::vector<cyclone::CollisionSphere*> _collisionsSpheres;
std::vector<cyclone::Joint*> _joints;
std::vector<cyclone::Spring*> _springs;
std::vector<cyclone::Contact*> _contacts;
cyclone::ContactResolver* _resolver;
cyclone::ForceRegistry* _forceRegistry;

int main(int argc, char** argv)
{
	GLFWwindow* window = InitWindow();

	
	GameObject* road = ModelLoader::LoadFromFile("assets/models/track/scene.gltf");
	GameObject* car = ModelLoader::LoadFromFile("assets/models/car/car.gltf");

	std::vector<GameObject*> gameObjects;
	gameObjects.push_back(car);
	gameObjects.push_back(road);
	ShaderProgram* shaderProgram = InitShader();
	car->SetShader(shaderProgram);
	car->AddComponent<CarController>();
	road->SetShader(shaderProgram);

	car->transform->Childs()[0]->SetScale(glm::vec3(1, 1, 1));

	double deltaTime = 0;
	double time = 0.f;
	glm::vec2 lastMouse(0, 0);
	//Draw(window, shaderProgram, gameObjects, viewer);

	while (!glfwWindowShouldClose(window))
	{
		auto start = std::chrono::system_clock::now();

		Draw(window, shaderProgram, gameObjects, viewer);

		glfwPollEvents();

		Mouse::Update();
		Keyboard::Update();

		for (auto obj : gameObjects) {
			obj->Update(static_cast<float>(deltaTime));
		}

		if (Keyboard::GetKeyState(GLFW_KEY_ESCAPE) == JUST_PRESSED)
			glfwSetWindowShouldClose(window, GL_TRUE);
		if (Mouse::Any(JUST_PRESSED))
			lastMouse = Mouse::cursor;
		MouseDragging(window, viewer, lastMouse);

		auto end = std::chrono::system_clock::now();
		std::chrono::duration<double> elapsed_seconds = end - start;
		deltaTime = elapsed_seconds.count();
		time += deltaTime;
	}


	return 0;
}