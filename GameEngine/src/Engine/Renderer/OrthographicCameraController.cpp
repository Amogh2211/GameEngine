#include "GameEnginepch.h"
#include "OrthographicCameraController.h"
#include "Engine/Core/Input.h"
#include "Engine/Core/KeyCodes.h"

namespace GameEngine {

	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotaiton)
		:m_AspectRatio(aspectRatio), m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio* m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), m_Rotation(rotaiton)
	{

	}

	void OrthographicCameraController::OnUpdate(Timestep ts) {

		if (Input::IsKeyPressed(GE_KEY_A)) {
			m_CameraPosition.x += m_CameraTransaltionSpeed * ts;
		}

		else if (Input::IsKeyPressed(GE_KEY_D)) {
			m_CameraPosition.x -= m_CameraTransaltionSpeed * ts;
		}


		if (Input::IsKeyPressed(GE_KEY_S)) {
			m_CameraPosition.y += m_CameraTransaltionSpeed * ts;
		}
		else if (Input::IsKeyPressed(GE_KEY_W)) {
			m_CameraPosition.y -= m_CameraTransaltionSpeed * ts;
		}

		if (m_Rotation) {
			if (Input::IsKeyPressed(GE_KEY_E)) {
				m_CameraRotation -= m_CameraRotationSpeed * ts;
			}
			if (Input::IsKeyPressed(GE_KEY_Q)) {
				m_CameraRotation += m_CameraRotationSpeed * ts;
			}
			m_Camera.SetRotation(m_CameraRotation);
		}
		m_Camera.SetPosition(m_CameraPosition);
		m_CameraTransaltionSpeed = m_ZoomLevel;
	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(GE_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(GE_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e) {

		m_ZoomLevel -= e.GetYOffset() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}


	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e) {
		float yScale = e.GetHeight() / 720.0f;

		m_AspectRatio -= (e.GetWidth() /  (float)e.GetHeight());
		
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel , m_AspectRatio * m_ZoomLevel , -m_ZoomLevel * yScale, m_ZoomLevel * yScale);
		return false;
	}

}