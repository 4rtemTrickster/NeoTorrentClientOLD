#include "NTCpch.h"
#include "Logger.h"

#include <spdlog/sinks/basic_file_sink.h>

namespace NTC
{
    Ref<spdlog::logger> Logger::BackendLogger;
    Ref<spdlog::logger> Logger::FrontEndLogger;

    void Logger::Init()
    {
        Ref<spdlog::sinks::basic_file_sink_mt> FileSink = CreateRef<spdlog::sinks::basic_file_sink_mt>("..\\NG.log", true);
        FileSink->set_pattern("%^[%T] [%n] [%l]: %v%$");

        BackendLogger = CreateRef<spdlog::logger>("Backend", FileSink);
        FrontEndLogger = CreateRef<spdlog::logger>("Frontend", FileSink);
        
        spdlog::register_logger(BackendLogger);
        spdlog::register_logger(FrontEndLogger);
        
        BackendLogger->set_level(spdlog::level::trace);
        BackendLogger->flush_on(spdlog::level::trace);

        FrontEndLogger->set_level(spdlog::level::trace);
        FrontEndLogger->flush_on(spdlog::level::trace);

        NTC_TRACE("Logger initialized!");
    }
}