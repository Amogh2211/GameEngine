#pragma once
//THIS WILL HAVE NO STORAGE ONLY A SET OF RENDER COMMANDS
#include "OrthographicCamera.h"
#include "Texture.h"

namespace GameEngine {
	class Renderer2D {
	public:

		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		// Primitives
		static void DrawQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& colour);
		static void DrawQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& colour);
		static void DrawQuad(const glm::vec2& pos, const glm::vec2& size, const std::shared_ptr<Texture2D>& texture);
		static void DrawQuad(const glm::vec3& pos, const glm::vec2& size, const std::shared_ptr<Texture2D>& texture);


	};
}