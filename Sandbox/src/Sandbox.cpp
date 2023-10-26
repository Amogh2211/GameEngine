
#include "Engine.h"
#include "imgui/imgui.h"

class ExampleLayer : public GameEngine::Layer {
public:
	ExampleLayer()
		: Layer("Example")
	{

	}

	void OnUpdate() override {
		GE_INFO("ExampleLayer::Update");

		if (GameEngine::Input::IsKeyPressed(GE_KEY_TAB))
			GE_INFO("Tab key is pressed");
	}

	
	void OnEvent(GameEngine::Event& event) override
	{
		
	}

};

class Sandbox : public GameEngine::Application {
public:
	Sandbox() {
		PushLayer(new ExampleLayer);
		PushOverlay(new GameEngine::ImGuiLayer());
	}
	~Sandbox() {

	}
};

GameEngine::Application* GameEngine::CreateApplication() {
	return new Sandbox();
}