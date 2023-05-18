#include "Graphics.h"

#include "Input.h"
#include "ShaderProgram.h"
#include "Console.hpp"

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

using namespace Module;

const glm::vec3 DEFAULT_VIEW_POINT(20, 20, 20);
const glm::vec3 DEFAULT_VIEW_CENTER(0, 0, 0);
const glm::vec3 DEFAULT_UP_VECTOR(0, 1, 0);

const glm::vec4 lightPos(10, 5, 10, 1);
const glm::vec3 La(1, 1, 1);
const glm::vec3 Ld(1, 1, 1);
const glm::vec3 Ls(1, 1, 1);

const GLfloat shiness = 10;
const glm::vec3 Ka(0.5, 0.5, 0.5);
const glm::vec3 Kd(0.9, 0.9, 0.9);
const glm::vec3 Ks(0.1, 0.1, 0.1);

Graphics::Graphics(float width, float height)
	:	m_window(nullptr),
		m_shaderProgram(nullptr),
		m_viewer(
			DEFAULT_VIEW_POINT,
			DEFAULT_VIEW_CENTER,
			DEFAULT_UP_VECTOR,
			glm::radians(45.0f),
			width / height
		),
		m_width(width),
		m_height(height)
{
	;
}

Graphics::~Graphics()
{
	delete m_shaderProgram;
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

void Graphics::Init()
{
}

void Graphics::InitWindow()
{
	if (!glfwInit())
		throw std::runtime_error("Failed to initialize glfw.");
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_window = glfwCreateWindow(m_width, m_height, "PatrackMania", NULL, NULL);
	if (!m_window)
	{
		glfwTerminate();
		throw std::runtime_error("Failed to create window.");
	}
	glfwMakeContextCurrent(m_window);

	if (gl3wInit()) throw std::runtime_error("Failed to initialize OpenGL.");
	if (!gl3wIsSupported(4, 2)) throw std::runtime_error("OpenGL 4.2 not supported");

	Console.LogFormat("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));
	glfwSwapInterval(1);

	glfwSetKeyCallback(m_window, Keyboard::KeyCallback);
	glfwSetMouseButtonCallback(m_window, Mouse::ButtonCallback);
	glfwSetCursorPosCallback(m_window, Mouse::CursorPositionCallBack);
}

void Graphics::InitShader()
{
	m_shaderProgram = new ShaderProgram();
	m_shaderProgram->initFromFiles("assets/shaders/simple.vert", "assets/shaders/simple.frag");

	m_shaderProgram->addUniform("Light.Position");
	m_shaderProgram->addUniform("Light.La");
	m_shaderProgram->addUniform("Light.Ld");
	m_shaderProgram->addUniform("Light.Ls");

	m_shaderProgram->addUniform("Material.Ka");
	m_shaderProgram->addUniform("Material.Kd");
	m_shaderProgram->addUniform("Material.Ks");
	m_shaderProgram->addUniform("Material.Shiness");

	m_shaderProgram->addUniform("ModelViewMatrix");
	m_shaderProgram->addUniform("NormalMatrix");
	m_shaderProgram->addUniform("MVP");

	m_shaderProgram->addUniform("TextureSampler");
	m_shaderProgram->addUniform("UseTexture");
}

void Graphics::Update(float deltaTime, std::vector<GameObject*> objects)
{
	glfwSwapBuffers(m_window);

	glClearColor(0.2f, 0.2f, .2f, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, m_width, m_height);

	glm::vec3 eye = m_viewer.getViewPoint();
	glm::vec3 look = m_viewer.getViewCenter();
	glm::vec3 up = m_viewer.getUpVector();
	m_view = glm::lookAt(eye, look, up);
	glEnable(GL_DEPTH_TEST);

	m_shaderProgram->use();
	m_projection = glm::perspective(glm::radians(45.0f), 1.0f * m_width / (float)m_height, 0.1f, 500.0f);

	glUniform4fv(m_shaderProgram->uniform("Light.Position"), 1, glm::value_ptr(lightPos));
	glUniform3fv(m_shaderProgram->uniform("Light.La"), 1, glm::value_ptr(La));
	glUniform3fv(m_shaderProgram->uniform("Light.Ld"), 1, glm::value_ptr(Ld));
	glUniform3fv(m_shaderProgram->uniform("Light.Ls"), 1, glm::value_ptr(Ls));

	glUniform3fv(m_shaderProgram->uniform("Material.Ka"), 1, glm::value_ptr(Ka));
	glUniform3fv(m_shaderProgram->uniform("Material.Kd"), 1, glm::value_ptr(Kd));
	glUniform3fv(m_shaderProgram->uniform("Material.Ks"), 1, glm::value_ptr(Ks));
	glUniform1fv(m_shaderProgram->uniform("Material.Shiness"), 1, &shiness);
}

const glm::mat4& Graphics::GetCameraView() const { return m_view; }

const glm::mat4& Graphics::GetCameraProjection() const { return m_projection; }
const ShaderProgram* Graphics::GetShader() const { return m_shaderProgram; }