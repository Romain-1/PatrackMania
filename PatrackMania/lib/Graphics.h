#pragma once

#include "IModule.h"
#include "Viewer.h"

class GLFWwindow;
class ShaderProgram;

namespace Module
{
	class Graphics : IModule
	{
	private:
		GLFWwindow* m_window;
		ShaderProgram* m_shaderProgram;
		Viewer m_viewer;
		float m_width;
		float m_height;

		glm::mat4 m_view;
		glm::mat4 m_projection;

	public:
		Graphics(float width, float height);
		~Graphics();

		void Init() override;
		void Update(float deltaTime, std::vector<GameObject*> objects) override;

		const glm::mat4 &GetCameraView() const;
		const glm::mat4 &GetCameraProjection() const;
		const ShaderProgram* GetShader() const;

	private:
		void InitWindow();
		void InitShader();
	};
}

