#pragma once

#include "IModule.h"
//#include "lib/Viewer.h"

class GLFWwindow;
class ShaderProgram;
class Viewer;

namespace Module
{
	class Graphics : public IModule
	{
	private:
		GLFWwindow* m_window;
		ShaderProgram* m_shaderProgram;
		float m_width;
		float m_height;

		glm::mat4 m_view;
		glm::mat4 m_projection;

	public:
		Viewer* m_viewer; // TODO implement cameras
		Graphics(float width, float height);
		~Graphics();

		void Init() override;
		void Update(float deltaTime, const std::vector<GameObject*> &objects) override;

		const glm::mat4 &GetCameraView() const;
		const glm::mat4 &GetCameraProjection() const;
		ShaderProgram* GetShader() const;
		bool WindowShouldClose() const;

	private:
		void InitWindow();
		void InitShader();
		static void WindowCloseEvent(GLFWwindow* window);
	};
}

