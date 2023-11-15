
#include "Engine.h"
#include "Engine/Core/EntryPoint.h"


#include "Platform/OpenGL/OpenGLShader.h"
#include "imgui/imgui.h"

#include "GLM/gtc/matrix_transform.hpp"
#include <GLM/gtc/type_ptr.hpp>

#include "Engine/Renderer/Shader.h"

#include "Sandbox2D.h"

class ExampleLayer : public GameEngine::Layer {
public:
	ExampleLayer()
		: Layer("Example"), m_CameraController(1280.0f / 720.0f)
	{


		m_VertexArray = GameEngine::VertexArray::Create();
		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f,  /* a_Color */  1.0f, 0.25f, 0.25f, 1.0f,
			0.5f, -0.5f, 0.0f,   /* a_Color */  0.25f, 1.0f, 0.25f, 1.0f,
			0.0f, 0.5f, 0.0f,    /* a_Color */  0.25f, 0.25f, 1.0f, 1.0f
		};

		std::shared_ptr<GameEngine::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(GameEngine::VertexBuffer::Create(vertices, sizeof(vertices)));

		GameEngine::BufferLayout layout = {
			{ GameEngine::ShaderDataType::Float3, "a_Position" },
			{ GameEngine::ShaderDataType::Float4, "a_Color" }
		};

		vertexBuffer->SetLayout(layout);

		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<GameEngine::IndexBuffer> indexBuffer;
		indexBuffer.reset(GameEngine::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);


		m_SquareVA = GameEngine::VertexArray::Create();

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		std::shared_ptr<GameEngine::VertexBuffer> squareVB;
		squareVB.reset(GameEngine::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ GameEngine::ShaderDataType::Float3, "a_Position" },
			{ GameEngine::ShaderDataType::Float2, "a_TextCoord" }
			});
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };

		std::shared_ptr<GameEngine::IndexBuffer> squareIB;
		squareIB.reset(GameEngine::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);


		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform *  vec4(a_Position, 1.0);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = vec4(v_Position*0.5+0.5, 1.0);
				color = v_Color;
			}
		)";

		m_Shader = GameEngine::Shader::Create("VertexPosTriangle" , vertexSrc, fragmentSrc);

		

		std::string FlatColourShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			
			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string FlatColourShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			uniform vec3 u_Colour;

			void main()
			{
				color = vec4(u_Colour, 1.0f);
			}
		)";

		m_FlatColourShader = GameEngine::Shader::Create("FlatColour" , FlatColourShaderVertexSrc, FlatColourShaderFragmentSrc);


		//// NEW SHADER /////

		

		auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");

		m_Texture = (GameEngine::Texture2D::Create("assets/textures/Checkerboard.png"));
		m_ChernoTexture = (GameEngine::Texture2D::Create("assets/textures/ChernoLogo.png"));

		std::dynamic_pointer_cast<GameEngine::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<GameEngine::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(GameEngine::Timestep ts) override {
		// GE_INFO("ExampleLayer::Update");

		m_CameraController.OnUpdate(ts);


		GameEngine::RenderCommand::SetClearColor({ (float)15 / 255, (float)15 / 255, (float)15 / 255, 1 });
		GameEngine::RenderCommand::Clear();


		GameEngine::Renderer::BeginScene(m_CameraController.GetCamera());

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		std::dynamic_pointer_cast<GameEngine::OpenGLShader>(m_FlatColourShader)->Bind();
		std::dynamic_pointer_cast<GameEngine::OpenGLShader>(m_FlatColourShader)->UploadUniformFloat3("u_Colour", m_SquareColour);

		// GRID RENDER
		for (int y = 0; y < 20; ++y) {
			for (int x = 0; x < 20; x++) {
				glm::vec3 position(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * scale;
				GameEngine::Renderer::Submit(m_SquareVA, m_FlatColourShader, transform);	
			}

		}

		auto textureShader = m_ShaderLibrary.Get("Texture");

		m_Texture->Bind();
		GameEngine::Renderer::Submit(m_SquareVA, textureShader, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		m_ChernoTexture->Bind();
		GameEngine::Renderer::Submit(m_SquareVA, textureShader, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		// TRIANGLE RENDER
		//GameEngine::Renderer::Submit(m_VertexArray, m_Shader);

		GameEngine::Renderer::EndScene();
	}

	
	void OnEvent(GameEngine::Event& event) override
	{
		m_CameraController.OnEvent(event);
		
	}



	virtual void OnImGuiRender() override {
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Colour", glm::value_ptr(m_SquareColour));
		ImGui::End();
	}

private:

	GameEngine::ShaderLibrary m_ShaderLibrary;
	std::shared_ptr<GameEngine::Shader> m_Shader;
	std::shared_ptr<GameEngine::VertexArray> m_VertexArray;

	std::shared_ptr<GameEngine::Shader> m_FlatColourShader;
	std::shared_ptr<GameEngine::VertexArray> m_SquareVA;

	std::shared_ptr<GameEngine::Texture2D> m_Texture, m_ChernoTexture;

	GameEngine::OrthographicCameraController m_CameraController;

	glm::vec3 m_SquareColour = { 0.2f, 0.3f, 0.4f };
};

class Sandbox : public GameEngine::Application {
public:
	Sandbox() {
		// PushLayer(new ExampleLayer());
		PushLayer(new Sandbox2D());
		//PushLayer(new GameEngine::ImGuiLayer());
	}
	~Sandbox() {

	}
};

GameEngine::Application* GameEngine::CreateApplication() {
	return new Sandbox();
}