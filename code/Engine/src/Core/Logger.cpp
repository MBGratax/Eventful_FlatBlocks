
#include "Include/Core/Logger.h"
#include <iostream>

namespace EventfulEngine::Debug {
    std::shared_ptr<spdlog::logger> Logger::s_Logger;

    void Logger::Init() {
        try {
            // Create a colored console sink.
            const auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
            // Create a basic file sink.
            const auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("EventfulEngine.log", true);

            std::vector<spdlog::sink_ptr> sinks { console_sink, file_sink };

            s_Logger = std::make_shared<spdlog::logger>("engine", sinks.begin(), sinks.end());
            spdlog::register_logger(s_Logger);

            // Set default log level.
            s_Logger->set_level(spdlog::level::info);
            s_Logger->flush_on(spdlog::level::info);
        } catch (const spdlog::spdlog_ex& ex) {
            std::cerr << "Logger initialization failed: " << ex.what() << std::endl;
        }
    }
}