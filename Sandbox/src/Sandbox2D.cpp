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


	m_SquareVA = GameEngine::VertexArray::Create();

	float squareVertices[5 * 4] = {
		-0.5f, -0.5f, 0.0f, 
		 0.5f, -0.5f, 0.0f, 
		 0.5f,  0.5f, 0.0f, 
		-0.5f,  0.5f, 0.0f
	};

	std::shared_ptr<GameEngine::VertexBuffer> squareVB;
	squareVB.reset(GameEngine::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
	squareVB->SetLayout({
		{ GameEngine::ShaderDataType::Float3, "a_Position" }
		});
	m_SquareVA->AddVertexBuffer(squareVB);

	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };

	std::shared_ptr<GameEngine::IndexBuffer> squareIB;
	squareIB.reset(GameEngine::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
	m_SquareVA->SetIndexBuffer(squareIB);

	m_FlatColourShader = GameEngine::Shader::Create("assets/shaders/FlatColourShader.glsl");
}

void Sandbox2D::OnDetach() {

}

void Sandbox2D::OnUpdate(GameEngine::Timestep ts) {

	m_CameraController.OnUpdate(ts);

	GameEngine::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	GameEngine::RenderCommand::Clear();


	GameEngine::Renderer::BeginScene(m_CameraController.GetCamera());

	std::dynamic_pointer_cast<GameEngine::OpenGLShader>(m_FlatColourShader)->Bind();
	std::dynamic_pointer_cast<GameEngine::OpenGLShader>(m_FlatColourShader)->UploadUniformFloat4("u_Color", m_SquareColour);


	GameEngine::Renderer::Submit(m_SquareVA, m_FlatColourShader, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	GameEngine::Renderer::EndScene();
}

void Sandbox2D::OnImGuiRender() {
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Colour", glm::value_ptr(m_SquareColour));
	ImGui::End();
}
void Sandbox2D::OnEvent(GameEngine::Event& event) {
	m_CameraController.OnEvent(event);
}