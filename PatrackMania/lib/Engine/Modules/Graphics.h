#pragma once

#include "IModule.h"
#include "lib/Engine/Ressources/Data/SkyBox.h"
//#include "lib/Viewer.h"

#include "../BaseComponents/Camera.h"

class GLFWwindow;
class Shader;
class Viewer;

namespace Module
{
	class Graphics : public IModule
	{
	private:
		GLFWwindow* m_window;
		Shader* m_shader;
		float m_width;
		float m_height;

		glm::mat4 m_view;
		glm::mat4 m_projection;
		Camera* mainCamera;

	public:
		//Viewer* m_viewer; // TODO implement cameras
		SkyBox* skybox;

	public:
		Graphics(float width, float height);
		~Graphics();

		void Init() override;
		void Update(float deltaTime, const std::vector<GameObject*> &objects) override;
		
		void SetMainCamera(Camera *camera);
		Camera* MainCamera() const;

		const glm::mat4 &GetCameraView() const;
		const glm::mat4 &GetCameraProjection() const;
		Shader* GetShader() const;
		bool WindowShouldClose() const;
		const glm::vec2 GetWindowSize() const;

	private:
		void InitWindow();
		void InitShader();
		static void WindowCloseEvent(GLFWwindow* window);
	};
}

