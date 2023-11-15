#include "Sandbox2D.h"
#include "imgui/imgui.h"
#include "GLM/gtc/matrix_transform.hpp"
#include <GLM/gtc/type_ptr.hpp>
#include "Platform/OpenGL/OpenGLShader.h"

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
{

}
void Sandbox2D::OnAttach() {

}

void Sandbox2D::OnDetach() {

}

void Sandbox2D::OnUpdate(GameEngine::Timestep ts) {

	m_CameraController.OnUpdate(ts);

	GameEngine::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	GameEngine::RenderCommand::Clear();


	GameEngine::Renderer2D::BeginScene(m_CameraController.GetCamera());

	//std::dynamic_pointer_cast<GameEngine::OpenGLShader>(m_FlatColourShader)->Bind();
	//std::dynamic_pointer_cast<GameEngine::OpenGLShader>(m_FlatColourShader)->UploadUniformFloat4("u_Color", m_SquareColour);

	GameEngine::Renderer2D::DrawQuad({0.0f,0.0f}, {1.0f,1.0f}, {0.8f, 0.2f,0.3f, 1.0f});
	GameEngine::Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender() {
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Colour", glm::value_ptr(m_SquareColour));
	ImGui::End();
}
void Sandbox2D::OnEvent(GameEngine::Event& event) {
	m_CameraController.OnEvent(event);
}