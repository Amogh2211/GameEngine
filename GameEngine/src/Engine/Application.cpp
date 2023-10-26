#include "GameEnginepch.h"


#include "Application.h"
#include "Events/Event.h"
#include "Log.h"
#include "Input.h"

#include "glad/glad.h"



namespace GameEngine {


#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		GE_CORE_ASSERT(!s_Instance, "Application already exists.")
		s_Instance = this;
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
	}

	Application::~Application() {

	}

	void Application::PushLayer(Layer* layer) {
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::OnEvent(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
		
		for (auto i = m_LayerStack.end(); i != m_LayerStack.begin();) {
			(*--i)->OnEvent(e);
			if (e.m_Handled)
				break;
		}
	}
	
	void Application::Run() {
		while (m_Running)
		{
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();
			auto x = Input::GetMouseX(), y = Input::GetMouseY();

			GE_CORE_TRACE("{0}, {1}", x, y);
			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e){
		m_Running = false;
		return true;

	}
}