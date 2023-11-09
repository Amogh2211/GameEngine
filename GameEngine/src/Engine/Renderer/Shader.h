#pragma once
#include <string>
#include <GLM/glm.hpp>

namespace GameEngine {
	class Shader {
	public:
		Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
		~Shader();

		void Bind() const;
		void ShaderUnbind() const;

		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);

	private:
		uint32_t m_RendererID;

	};
}

