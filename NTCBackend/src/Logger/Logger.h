#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace NTC
{
    class Logger
    {
    public:
        static void Init();

        static Ref<spdlog::logger>& GetBackendLogger() { return BackendLogger; }
        static Ref<spdlog::logger>& GetFrontendLogger() { return FrontEndLogger; }

    private:
        
        static Ref<spdlog::logger> BackendLogger;
        static Ref<spdlog::logger> FrontEndLogger;
    };
}

// DO NOT USE INSIDE CONSTRUCTORS IT CAUSES CRASH!!!!!!!!

#define NTC_TRACE(...)      ::NTC::Logger::GetBackendLogger()->trace(__VA_ARGS__)
#define NTC_INFO(...)       ::NTC::Logger::GetBackendLogger()->info(__VA_ARGS__)
#define NTC_WARN(...)       ::NTC::Logger::GetBackendLogger()->warn(__VA_ARGS__)
#define NTC_ERROR(...)      ::NTC::Logger::GetBackendLogger()->error(__VA_ARGS__)
#define NTC_CRITICAL(...)   ::NTC::Logger::GetBackendLogger()->critical(__VA_ARGS__)

#define FRONTEND_TRACE(...)      ::NTC::Logger::GetFrontendLogger()->trace(__VA_ARGS__)
#define FRONTEND_INFO(...)       ::NTC::Logger::GetFrontendLogger()->info(__VA_ARGS__)
#define FRONTEND_WARN(...)       ::NTC::Logger::GetFrontendLogger()->warn(__VA_ARGS__)
#define FRONTEND_ERROR(...)      ::NTC::Logger::GetFrontendLogger()->error(__VA_ARGS__)
#define FRONTEND_CRITICAL(...)   ::NTC::Logger::GetFrontendLogger()->critical(__VA_ARGS__)

#ifdef IPC_LOGGING
    #define LOG_MESSAGE(...)    ::NTC::Logger::GetBackendLogger()->trace(__VA_ARGS__)
#else
    #define LOG_MESSAGE(...)
#endif
