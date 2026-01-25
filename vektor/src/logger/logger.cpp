#include "logger/logger.hpp"

#include <vector>
#include <filesystem>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace vektor::logger
{
    std::shared_ptr<spdlog::logger> Logger::s_coreLogger;
    std::shared_ptr<spdlog::logger> Logger::s_clientLogger;

    void Logger::Init()
    {
        std::vector<spdlog::sink_ptr> logSinks;

        auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        consoleSink->set_pattern("%^[%T] %n: %v%$");
        logSinks.emplace_back(consoleSink);

        std::string log_folder_name = "logs";
        std::filesystem::create_directory(log_folder_name);

        auto fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(log_folder_name + "/engine.log", true);
        fileSink->set_pattern("[%T] [%l] %n: %v");
        logSinks.emplace_back(fileSink);

        s_coreLogger = std::make_shared<spdlog::logger>("VEKTOR", begin(logSinks), end(logSinks));
        spdlog::register_logger(s_coreLogger);
        s_coreLogger->set_level(spdlog::level::trace);
        s_coreLogger->flush_on(spdlog::level::trace);

        s_clientLogger = std::make_shared<spdlog::logger>("APP", begin(logSinks), end(logSinks));
        spdlog::register_logger(s_clientLogger);
        s_clientLogger->set_level(spdlog::level::trace);
        s_clientLogger->flush_on(spdlog::level::trace);
    }

    void Logger::Shutdown()
    {
        spdlog::shutdown();
    }
}