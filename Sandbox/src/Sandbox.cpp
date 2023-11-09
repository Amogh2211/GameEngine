
#include "Engine.h"
#include "imgui/imgui.h"

class ExampleLayer : public GameEngine::Layer {
public:
	ExampleLayer()
		: Layer("Example"), m_Camera(-2.0f, 2.0f, -2.0f, 2.0f), m_CameraPosition(0.0f)
	{


		m_VertexArray.reset(GameEngine::VertexArray::Create());
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


		m_SquareVA.reset(GameEngine::VertexArray::Create());

		float squareVertices[3 * 4] = {
			-0.75f, -0.75f, 0.0f,
			0.75f, -0.75f, 0.0f,
			0.75f, 0.75f, 0.0f,
			-0.75f, 0.75f, 0.0f
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


		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
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

		m_Shader.reset(new GameEngine::Shader(vertexSrc, fragmentSrc));


		std::string BlueShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			
			uniform mat4 u_ViewProjection;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
			}
		)";

		std::string BlueShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			void main()
			{
				color = vec4(0.25, 0., 1, 1.0);
			}
		)";

		m_BlueShader.reset(new GameEngine::Shader(BlueShaderVertexSrc, BlueShaderFragmentSrc));
	}

	void OnUpdate() override {
		// GE_INFO("ExampleLayer::Update");

		if (GameEngine::Input::IsKeyPressed(GE_KEY_LEFT) ) {
			m_CameraPosition.x -= m_CameraMoveSpeed;
		}

		else if (GameEngine::Input::IsKeyPressed(GE_KEY_RIGHT)) {
			m_CameraPosition.x += m_CameraMoveSpeed;
		}


		if (GameEngine::Input::IsKeyPressed(GE_KEY_DOWN)) {
			m_CameraPosition.y -= m_CameraMoveSpeed;
		}
		else if (GameEngine::Input::IsKeyPressed(GE_KEY_UP)) {
			m_CameraPosition.y += m_CameraMoveSpeed;
		}

		if (GameEngine::Input::IsKeyPressed(GE_KEY_E)) {
			m_CameraRotation -= m_CameraRotationSpeed;
		}
		if (GameEngine::Input::IsKeyPressed(GE_KEY_Q)) {
			m_CameraRotation += m_CameraRotationSpeed;
		}

		GameEngine::RenderCommand::SetClearColor({ (float)15 / 255, (float)15 / 255, (float)15 / 255, 1 });
		GameEngine::RenderCommand::Clear();


		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);


		GameEngine::Renderer::BeginScene(m_Camera);

		GameEngine::Renderer::Submit(m_SquareVA, m_BlueShader);
		GameEngine::Renderer::Submit(m_VertexArray, m_Shader);

		GameEngine::Renderer::EndScene();
	}

	
	void OnEvent(GameEngine::Event& event) override
	{
		
	}



	virtual void OnImGuiRender() override {
	}

private:
	std::shared_ptr<GameEngine::Shader> m_Shader;
	std::shared_ptr<GameEngine::VertexArray> m_VertexArray;

	std::shared_ptr<GameEngine::Shader> m_BlueShader;
	std::shared_ptr<GameEngine::VertexArray> m_SquareVA;

	GameEngine::OrthographicCamera m_Camera;

	glm::vec3 m_CameraPosition;
	float m_CameraMoveSpeed = 0.01f;

	float m_CameraRotation = 0.0f;
	float m_CameraRotationSpeed = 0.01f;
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