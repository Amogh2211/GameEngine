#pragma once

#include <memory>
#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"


namespace GameEngine {

	class GE_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;

	};
}

// CORE LOG MACROS

#define GE_CORE_TRACE(...) ::GameEngine::Log::GetCoreLogger()->trace(__VA_ARGS__);
#define GE_CORE_INFO(...) ::GameEngine::Log::GetCoreLogger()->info(__VA_ARGS__);
#define GE_CORE_WARN(...) ::GameEngine::Log::GetCoreLogger()->warn(__VA_ARGS__);
#define GE_CORE_ERROR(...) ::GameEngine::Log::GetCoreLogger()->error(__VA_ARGS__);
#define GE_CORE_FATAL(...) ::GameEngine::Log::GetCoreLogger()->fatal(__VA_ARGS__);

// CLIENT LOG MACROS

#define GE_TRACE(...) ::GameEngine::Log::GetCoreLogger()->trace(__VA_ARGS__);
#define GE_INFO(...)  ::GameEngine::Log::GetCoreLogger()->info(__VA_ARGS__);
#define GE_WARN(...)  ::GameEngine::Log::GetCoreLogger()->warn(__VA_ARGS__);
#define GE_ERROR(...) ::GameEngine::Log::GetCoreLogger()->error(__VA_ARGS__);
#define GE_FATAL(...) ::GameEngine::Log::GetCoreLogger()->fatal(__VA_ARGS__);
