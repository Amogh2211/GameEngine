#pragma once
#include "Core.h"
#include "Window.h"
#include "Engine/Core/Timestep.h"

// Events includes
#include "Engine/Core/LayerStack.h"
#include "Engine/Events/Event.h"
#include "Engine/Events/ApplicationEvent.h"
#include "Engine/ImGui/ImGuiLayer.h"

// Render Includes
#include "Engine/Renderer/Shader.h"
#include "Engine/Renderer/Buffer.h"
#include "Engine/Renderer/VertexArray.h"



namespace GameEngine {

	class GE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();	
		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *m_Window;  }
		inline static Application& Get() { return *s_Instance; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		float m_LastFrameTime = 0.0f;


		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;

		LayerStack m_LayerStack;


	private:
		static Application* s_Instance;
	};


	// To be defined in client

	Application* CreateApplication();


}

