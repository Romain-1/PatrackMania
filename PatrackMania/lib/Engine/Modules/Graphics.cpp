#include <GL/gl3w.h>

#include "Graphics.h"

#include "lib/Console.hpp"
#include "lib/Viewer.h"
#include "lib/Engine/Engine.h"
#include "lib/Engine/Input.h"
#include "lib/Engine/Ressources/Data/Shader.h"
#include "lib/Engine/BaseComponents/MeshRenderer.hpp"
#include "lib/Engine/BaseComponents/Camera.h"

#include <glm/gtc/type_ptr.hpp>

using namespace Module;

const glm::vec3 DEFAULT_VIEW_POINT(10, 10, 10);
const glm::vec3 DEFAULT_VIEW_CENTER(0, 0, 0);
const glm::vec3 DEFAULT_UP_VECTOR(0, 1, 0);

Graphics::Graphics(float width, float height) :
	m_window(nullptr),
	m_shader(nullptr),
	//m_viewer(nullptr),
	m_width(width),
	m_height(height),
	skybox(nullptr)
{
	InitWindow();
	InitShader();
	//m_viewer = new Viewer(
	//	DEFAULT_VIEW_POINT,
	//	DEFAULT_VIEW_CENTER,
	//	DEFAULT_UP_VECTOR,
	//	glm::radians(45.0f),
	//	m_width / m_height
	//);
	
	GameObject* cameraObj = new GameObject("DefaultMainCamera", glm::vec3(0, 2, 0));
	SetMainCamera(cameraObj->AddComponent<Camera>());
}

Graphics::~Graphics()
{
	delete m_shader;
	//delete m_viewer;
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

void Graphics::Init()
{
}

void Graphics::InitWindow()
{
	Console.Log(__FILE__, __func__, __LINE__);
	if (!glfwInit())
		throw std::runtime_error("Failed to initialize glfw.");
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	Console.Log(__FILE__, __func__, __LINE__);

	m_window = glfwCreateWindow(m_width, m_height, "PatrackMania", NULL, NULL);
	if (!m_window)
	{
		glfwTerminate();
		throw std::runtime_error("Failed to create window.");
	}
	Console.Log(__FILE__, __func__, __LINE__);
	glfwMakeContextCurrent(m_window);

	if (gl3wInit()) throw std::runtime_error("Failed to initialize OpenGL.");
	if (!gl3wIsSupported(4, 2)) throw std::runtime_error("OpenGL 4.2 not supported");

	Console.LogFormat("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));
	glfwSwapInterval(1);

	glfwSetKeyCallback(m_window, Keyboard::KeyCallback);
	glfwSetMouseButtonCallback(m_window, Mouse::ButtonCallback);
	glfwSetCursorPosCallback(m_window, Mouse::CursorPositionCallBack);
	glfwSetWindowCloseCallback(m_window, Graphics::WindowCloseEvent);
}

void Graphics::InitShader()
{
	m_shader = new Shader();
	m_shader->initFromFiles("assets/shaders/simple.vert", "assets/shaders/simple.frag");

	m_shader->addUniform("Light.Position");
	m_shader->addUniform("Light.La");
	m_shader->addUniform("Light.Ld");
	m_shader->addUniform("Light.Ls");

	m_shader->addUniform("Material.Ka");
	m_shader->addUniform("Material.Kd");
	m_shader->addUniform("Material.Ks");
	m_shader->addUniform("Material.Shiness");

	m_shader->addUniform("ModelViewMatrix");
	m_shader->addUniform("NormalMatrix");
	m_shader->addUniform("MVP");

	m_shader->addUniform("TextureSampler");

	Material* defaultMaterial = new Material();
	Texture* defaultTexture = Engine::Ressources->LoadTextureFromFile("assets/textures/baseTexture.png");
	defaultMaterial->BaseColorTexture = defaultTexture;

	Engine::Ressources->AddMaterial("default", defaultMaterial);
}

#include <glm/gtx/string_cast.hpp>
void Graphics::Update(float deltaTime, const std::vector<GameObject*> &objects)
{
	glfwSwapBuffers(m_window);

	glClearColor(0.2f, 0.2f, .2f, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, m_width, m_height);

	glm::vec3 eye = mainCamera->transform->LocalPosition();//m_viewer->getViewPoint();
	glm::vec3 look = eye + mainCamera->transform->Forward();//m_viewer->getViewCenter();
	glm::vec3 up = mainCamera->transform->Up();//m_viewer->getUpVector();
	m_view = glm::lookAt(eye, look, up);

	if (skybox) skybox->Draw();

	glEnable(GL_DEPTH_TEST);
	for (auto go : objects) {
		go->ApplyOnHierarchy([&](GameObject *obj) {
			if (!obj->activeInHierarchy) return;
			MeshRenderer* renderer = obj->GetComponent<MeshRenderer>();
			if (renderer) {
				renderer->Draw();
			}
		});
	}

	// Update events

	glfwPollEvents();

	Mouse::Update();
	Keyboard::Update();
}

void Graphics::SetMainCamera(Camera* camera)
{
	m_projection = glm::perspective(glm::radians(camera->fov), m_width / m_height, camera->zNear, camera->zFar);
	mainCamera = camera;
}
Camera* Graphics::MainCamera() const { return mainCamera; }
const glm::mat4& Graphics::GetCameraView() const { return m_view; }
const glm::mat4& Graphics::GetCameraProjection() const { return m_projection; }
Shader* Graphics::GetShader() const { return m_shader; }
bool Graphics::WindowShouldClose() const { return glfwWindowShouldClose(m_window); }
void Graphics::WindowCloseEvent(GLFWwindow* window) { glfwSetWindowShouldClose(window, GL_TRUE); }
const glm::vec2 Graphics::GetWindowSize() const { return glm::vec2(m_width, m_height); }