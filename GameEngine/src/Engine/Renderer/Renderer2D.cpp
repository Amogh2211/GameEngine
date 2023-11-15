#include "GameEnginepch.h"
#include "Renderer2D.h"
#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace GameEngine {

	struct Renderer2DStore {
		std::shared_ptr<VertexArray> QuadVertexArray;
		std::shared_ptr<Shader> flatColourShader;
	};

	static Renderer2DStore* s_Data;

	void Renderer2D::Init() {
		s_Data = new Renderer2DStore();
		s_Data->QuadVertexArray = VertexArray::Create();

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};

		std::shared_ptr<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ ShaderDataType::Float3, "a_Position" }
			});
		s_Data->QuadVertexArray->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };

		std::shared_ptr<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		s_Data->QuadVertexArray->SetIndexBuffer(squareIB);

		s_Data->flatColourShader = Shader::Create("assets/shaders/FlatColourShader.glsl");
	 }

	void Renderer2D::Shutdown() {
		delete s_Data;
	 }

	void Renderer2D::BeginScene(const OrthographicCamera& camera) {
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->flatColourShader)->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->flatColourShader)->UploadUniformMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->flatColourShader)->UploadUniformMat4("u_Transform", glm::mat4(1.0f));
	}

	void Renderer2D::EndScene() {

	 }

	// Primitives
	void Renderer2D::DrawQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& colour) {
		DrawQuad({ pos.x, pos.y, 0.0f }, size, colour);
	}

	void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& colour) {

		std::dynamic_pointer_cast<OpenGLShader>(s_Data->flatColourShader)->Bind();
		std::dynamic_pointer_cast<GameEngine::OpenGLShader>(s_Data->flatColourShader)->UploadUniformFloat4("u_Color", colour);


		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	 }
}