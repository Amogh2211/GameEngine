#include "GameEnginepch.h"
#include "OpenGLContext.h"
#include "glad/glad.h"

#include <GLFW/glfw3.h>

namespace GameEngine {
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) 
		: m_WindowHandle(windowHandle)
	{
		GE_CORE_ASSERT(windowHandle, "WindowHandle is NULL!")
	}

	void OpenGLContext::Init() {
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		GE_CORE_ASSERT(status, "Failed to init GLAD.");

		GE_CORE_INFO("OpenGL Rendered by: {0}, {1}", (const char*)glGetString(GL_VENDOR), (const char*)glGetString(GL_RENDERER));
	}

	void OpenGLContext::SwapBuffer() {

		
		glfwSwapBuffers(m_WindowHandle);
	}
}