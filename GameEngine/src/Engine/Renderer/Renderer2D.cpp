#include "GameEnginepch.h"
#include "Renderer2D.h"
#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "GLM/gtc/matrix_transform.hpp"

namespace GameEngine {

	struct Renderer2DStore {
		std::shared_ptr<VertexArray> QuadVertexArray;
		std::shared_ptr<Shader> flatColourShader;
		std::shared_ptr<Shader> textureShader;

	};

	static Renderer2DStore* s_Data;

	void Renderer2D::Init() {
		s_Data = new Renderer2DStore();
		s_Data->QuadVertexArray = VertexArray::Create();

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		std::shared_ptr<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" },
			});
		s_Data->QuadVertexArray->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };

		std::shared_ptr<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		s_Data->QuadVertexArray->SetIndexBuffer(squareIB);

		s_Data->flatColourShader = Shader::Create("assets/shaders/FlatColourShader.glsl");
		s_Data->textureShader = Shader::Create("assets/shaders/Texture.glsl");
		s_Data->textureShader->Bind();
		s_Data->textureShader->SetInt("u_Texture", 0);
	 }

	void Renderer2D::Shutdown() {
		delete s_Data;
	 }

	void Renderer2D::BeginScene(const OrthographicCamera& camera) {
		(s_Data->flatColourShader)->Bind();
		(s_Data->flatColourShader)->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
		
		(s_Data->textureShader)->Bind();
		(s_Data->textureShader)->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
		
	}

	void Renderer2D::EndScene() {

	 }

	// Primitives
	void Renderer2D::DrawQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& colour) {
		DrawQuad({ pos.x, pos.y, 0.0f }, size, colour);
	}

	void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& colour) {

		s_Data->flatColourShader->Bind();
		(s_Data->flatColourShader)->SetFloat4("u_Color", colour);
		
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});
		(s_Data->flatColourShader)->SetMat4("u_Transform", transform);

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	 }

	void Renderer2D::DrawQuad(const glm::vec2& pos, const glm::vec2& size, const std::shared_ptr<Texture2D>& texture) {
		DrawQuad({ pos.x, pos.y, 0.0f }, size, texture);
	}

	void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2& size, const std::shared_ptr<Texture2D>& texture) {
		s_Data->textureShader->Bind();
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		(s_Data->textureShader)->SetMat4("u_Transform", transform);

		texture->Bind();
		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}
}