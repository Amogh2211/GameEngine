#pragma once
#include "Engine.h"


class Sandbox2D : public GameEngine::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(GameEngine::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(GameEngine::Event& event) override;

private:
	GameEngine::OrthographicCameraController m_CameraController;
	std::shared_ptr<GameEngine::Shader> m_FlatColourShader;
	std::shared_ptr<GameEngine::VertexArray> m_SquareVA;

	std::shared_ptr<GameEngine::Texture2D> m_CheckerBoardTexture;

	glm::vec4 m_SquareColour = { 0.2f, 0.3f, 0.8f, 1.0f };
};

