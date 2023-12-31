#pragma once

#include "Engine/Renderer/OrthographicCamera.h"
#include "Engine/Core/Timestep.h"

#include "Engine/Events/ApplicationEvent.h"
#include "Engine/Events/MouseEvent.h"

namespace GameEngine {


	class OrthographicCameraController {

	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false); // Can take aspect ratio as a single float


		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);
		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }

		float GetZoomLevel() const { return m_ZoomLevel; }
		void SetZoomLevel(float level) { m_ZoomLevel = level; }
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);


		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		OrthographicCamera m_Camera;

		bool m_Rotation = false;

		glm::vec3 m_CameraPosition = {0.0f, 0.0f, 0.0f} ;
		float m_CameraRotation = 0.0f;

		float m_CameraTransaltionSpeed = 1.0f, m_CameraRotationSpeed = 180.0f;
	};

}