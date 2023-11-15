#include "GameEnginepch.h"

#include "OpenGLShader.h"

#include <fstream>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace GameEngine {

	static GLenum ShaderTypeFromString(const std::string& type) {
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;

		GE_CORE_ASSERT(false, "Unknown shader type");
		return 0;
	}

	OpenGLShader::OpenGLShader(const std::string& filepath) {

		std::string source = ReadFile(filepath);
		auto shaderSource = PreProcess(source);
		Compile(shaderSource);

		// GET NAME OUT OF FILEPATH
		// 
		auto lastSlash = filepath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;

		auto lastDot = filepath.rfind(".");
		
		auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
		m_Name = filepath.substr(lastSlash, count);
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc) 
		: m_Name(name) {
		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragmentSrc;
		Compile(sources);

	}

	OpenGLShader::~OpenGLShader() {
		glDeleteProgram(m_RendererID);
	}

	std::string OpenGLShader::ReadFile(const std::string& filepath) {
		std::string result;

		std::ifstream in(filepath, std::ios::in, std::ios::binary);
		if (in) {

			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();

		}
		else {
			GE_CORE_ERROR("Could not open file '{0}'!", filepath);
		}
		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source) {
		
		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			GE_CORE_ASSERT(eol != std::string::npos, "Syntax Error");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);

			GE_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specification!"); // CHeck for spaces in your shader code

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);
			shaderSources[ShaderTypeFromString(type)] = source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
		}
		return shaderSources;	
	}

	void OpenGLShader::Compile(std::unordered_map<GLenum, std::string>& shaderSources) {
		
		GLuint program = glCreateProgram();
		GE_CORE_ASSERT((int)shaderSources.size() <= 2, "Too many shader sources. Only supports 2");
		
		std::array<GLenum, 2> glShaderID;
		int glShaderIDIndex = 0;

		for (auto& key : shaderSources) {
			GLenum type = key.first;
			const std::string& source = key.second;

			// Read our shaders into the appropriate buffers
			// Create an empty vertex shader handle

			GLuint shader = glCreateShader(type);

			// Send the vertex shader source code to GL
			// Note that std::string's .c_str is NULL character terminated.
			const GLchar* sourceCString = source.c_str();
			glShaderSource(shader, 1, &sourceCString, 0);

			// Compile the vertex shader
			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				// We don't need the shader anymore.
				glDeleteShader(shader);

				GE_CORE_ERROR("{0}", infoLog.data());
				GE_CORE_ASSERT(false, "Shader compilation error.");
				break;
			}
			glAttachShader(program, shader);
			glShaderID[glShaderIDIndex++] = (shader);
		}

		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);
			// Don't leak shaders either.
			for (auto& key : glShaderID) {
				glDeleteShader(key);
			}
		

			GE_CORE_ERROR("{0}", infoLog.data());
			GE_CORE_ASSERT(false, "Linking error.");
			return;
		}

		// Always detach shaders after a successful link.
		for(auto& key : glShaderID)
			glDetachShader(program, key);
	
		m_RendererID = program;
	}

	
	void OpenGLShader::Bind() const {
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::ShaderUnbind() const {
		glUseProgram(0);
	}

	void OpenGLShader::UploadUniformInt(const std::string& name, int values) {
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, values);

	}

	void OpenGLShader::UploadUniformFloat(const std::string& name, float values) {
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1f(location, values);
	}

	void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& values) {
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2f(location, values.x, values.y);
	}
	
	void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& values) {
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3f(location, values.x, values.y, values.z);
	}

	void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& values) {
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4f(location, values.x, values.y, values.z, values.w);
	}

	void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix) {

		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

}