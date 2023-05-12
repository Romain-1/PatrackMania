#include <stdint.h>
#include <iostream>
#include <chrono>


#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <tiny_gltf.h>

//#include <GL/glew.h>
#include "lib/Viewer.h"

#include <GL/gl3w.h>

#include <cyclone.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include "lib/Console.hpp"
#include "lib/MeshRenderer.hpp"
#include "lib/Input.h"
#include "lib/Loader.h"
#include "lib/GameObject.h"
//#include "lib/Texture.h"

#include "CarController.h"

const int WIDTH = 800;
const int HEIGHT = 600;

const glm::vec3 DEFAULT_VIEW_POINT(20, 20, 20);
const glm::vec3 DEFAULT_VIEW_CENTER(0, 0, 0);
const glm::vec3 DEFAULT_UP_VECTOR(0, 1, 0);

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
	glfwSwapBuffers(window);

	glClearColor(0.2f, 0.2f, .2f, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, WIDTH, HEIGHT);

	glm::vec3 eye = viewer.getViewPoint();
	glm::vec3 look = viewer.getViewCenter();
	glm::vec3 up = viewer.getUpVector();
	glm::mat4 view = glm::lookAt(eye, look, up);
	glEnable(GL_DEPTH_TEST);

	shaderProgram->use();
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1.0f * WIDTH / (float)HEIGHT, 0.1f, 500.0f);

	glm::vec4 lightPos(10, 5, 10, 1);
	glm::vec3 La(1, 1, 1);
	glm::vec3 Ld(1, 1, 1);
	glm::vec3 Ls(1, 1, 1);

	glUniform4fv(shaderProgram->uniform("Light.Position"), 1, glm::value_ptr(lightPos));
	glUniform3fv(shaderProgram->uniform("Light.La"), 1, glm::value_ptr(La));
	glUniform3fv(shaderProgram->uniform("Light.Ld"), 1, glm::value_ptr(Ld));
	glUniform3fv(shaderProgram->uniform("Light.Ls"), 1, glm::value_ptr(Ls));

	GLfloat shiness = 10;
	glm::vec3 Ka(0.5, 0.5, 0.5);
	glm::vec3 Kd(0.9, 0.9, 0.9);
	glm::vec3 Ks(0.1, 0.1, 0.1);

	glUniform3fv(shaderProgram->uniform("Material.Ka"), 1, glm::value_ptr(Ka));
	glUniform3fv(shaderProgram->uniform("Material.Kd"), 1, glm::value_ptr(Kd));
	glUniform3fv(shaderProgram->uniform("Material.Ks"), 1, glm::value_ptr(Ks));
	glUniform1fv(shaderProgram->uniform("Material.Shiness"), 1, &shiness);

	for (auto& gameObject : gameObjects) {
		if (gameObject->activeInHierarchy) {
			gameObject->Draw(view, projection);
		}
	}
}

GameObject* LoadModelRec(
	const tinygltf::Model& model,
	const tinygltf::Node &node,
	GameObject *parent
)
{
	Console.Log(node.name, "child of", parent ? parent->name : "null");
	Transform* tr = new Transform();

	if (parent) tr->SetParent(parent->transform);
	if (node.translation.size() == 3)	tr->SetPosition(glm::vec3(node.translation[0], node.translation[1], node.translation[2]));
	if (node.scale.size() == 3)			tr->SetScale(glm::vec3(node.scale[0], node.scale[1], node.scale[2]));
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
		tr->SetPosition(translation);
		tr->SetScale(scale);
		tr->SetRotation(rotation);
	}

	GameObject *go = new GameObject(node.name, tr);
	for (auto& child : node.children) {
		LoadModelRec(model, model.nodes[child], go);
	}
	if (node.mesh == -1) return go;
	go->meshRenderer = new MeshRenderer(model, node);
	return go;
}

GameObject *LoadModel(const std::string &name)
{
	tinygltf::Model model;
	tinygltf::TinyGLTF loader;
	std::string err;
	std::string warn;

	if (!loader.LoadASCIIFromFile(&model, &err, &warn, name)) {
		Console.Log(err, warn);
		return nullptr;
	}
	return LoadModelRec(model, model.nodes[0], nullptr);
}

GLFWwindow* InitWindow()
{
	if (!glfwInit())
		throw std::runtime_error("Failed to initialize glfw.");
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "PatrackMania", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		throw std::runtime_error("Failed to create window.");
	}
	glfwMakeContextCurrent(window);

	if (gl3wInit()) throw std::runtime_error("Failed to initialize OpenGL.");
	if (!gl3wIsSupported(4, 2)) throw std::runtime_error("OpenGL 4.2 not supported");

	Console.LogFormat("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));
	glfwSwapInterval(1);

	glfwSetKeyCallback(window, Keyboard::KeyCallback);
	glfwSetMouseButtonCallback(window, Mouse::ButtonCallback);
	glfwSetCursorPosCallback(window, Mouse::CursorPositionCallBack);
	return window;
}

ShaderProgram* InitShader()
{
	ShaderProgram* shaderProgram = new ShaderProgram();
	shaderProgram->initFromFiles("assets/shaders/simple.vert", "assets/shaders/simple.frag");

	shaderProgram->addUniform("Light.Position");
	shaderProgram->addUniform("Light.La");
	shaderProgram->addUniform("Light.Ld");
	shaderProgram->addUniform("Light.Ls");

	shaderProgram->addUniform("Material.Ka");
	shaderProgram->addUniform("Material.Kd");
	shaderProgram->addUniform("Material.Ks");
	shaderProgram->addUniform("Material.Shiness");

	shaderProgram->addUniform("ModelViewMatrix"); //View*Model : mat4
	shaderProgram->addUniform("NormalMatrix"); //Refer next slide : mat3
	shaderProgram->addUniform("MVP");

	shaderProgram->addUniform("TextureSampler");
	shaderProgram->addUniform("UseTexture");
	return shaderProgram;
}

int main(int argc, char** argv)
{
	GLFWwindow* window = InitWindow();

	Viewer viewer(
		DEFAULT_VIEW_POINT,
		DEFAULT_VIEW_CENTER,
		DEFAULT_UP_VECTOR,
		glm::radians(45.0f),
		(float)WIDTH / (float)HEIGHT
	);
	GameObject* road = LoadModel("assets/models/track/scene.gltf");
	GameObject* car = LoadModel("assets/models/car/car.gltf");

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

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}