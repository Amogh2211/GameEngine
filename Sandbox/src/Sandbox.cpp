
#include "Engine.h"
#include "imgui/imgui.h"

class ExampleLayer : public GameEngine::Layer {
public:
	ExampleLayer()
		: Layer("Example")
	{

	}

	void OnUpdate() override {
		// GE_INFO("ExampleLayer::Update");

		if (GameEngine::Input::IsKeyPressed(GE_KEY_TAB))
			GE_INFO("Tab key is pressed");
	}

	
	void OnEvent(GameEngine::Event& event) override
	{
		if (event.GetEventType() == GameEngine::EventType::KeyPressed)
		{
			GameEngine::KeyPressedEvent& e = (GameEngine::KeyPressedEvent&)event;
			if (e.GetKeyCode() == GE_KEY_TAB)
				GE_TRACE("Tab key is pressed (event)!");
			GE_TRACE("{0}", (char)e.GetKeyCode());
		}
	}

	virtual void OnImGuiRender() override {
		ImGui::Begin("Test");
		ImGui::Text("Hello World");
		ImGui::End();
	}

};

class Sandbox : public GameEngine::Application {
public:
	Sandbox() {
		PushLayer(new ExampleLayer());
		//PushLayer(new GameEngine::ImGuiLayer());
	}
	~Sandbox() {

	}
};

GameEngine::Application* GameEngine::CreateApplication() {
	return new Sandbox();
}