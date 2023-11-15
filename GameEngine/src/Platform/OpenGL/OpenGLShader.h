#pragma once
#include "Engine/Renderer/Shader.h"
#include <GLM/gtc/type_ptr.hpp>

typedef unsigned int GLenum;

namespace GameEngine {
	class OpenGLShader : public Shader{
	public:
		OpenGLShader(const std::string& filepath);
		OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void ShaderUnbind() const override;

		virtual const std::string& GetName() const override { return m_Name; };

		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);
		void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);

		void UploadUniformInt(const std::string& name,int value);

		void UploadUniformFloat(const std::string& name, float value);
		void UploadUniformFloat2(const std::string& name, const glm::vec2& value);
		void UploadUniformFloat3(const std::string& name, const glm::vec3& value);
		void UploadUniformFloat4(const std::string& name, const glm::vec4& value);

	private:
		uint32_t m_RendererID;
		std::string m_Name;

		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
		void Compile(std::unordered_map<GLenum, std::string>& shaderSources);
		std::string ReadFile(const std::string& filepath);


	};
}

