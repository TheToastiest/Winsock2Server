#include "Logger.h"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <iostream>

std::shared_ptr<spdlog::logger> Logger::logger = nullptr;

void Logger::init() {
    try {
        // Create a basic file sink that writes to "basic_log.txt".
        auto basic_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("basic_log.txt", true);
        // Create a rotating file sink that writes to "rotating_log.txt".
        // This sink rotates the file when it reaches 5 MB, preserving up to 3 old files.
        auto rotating_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("rotating_log.txt", 1024 * 1024 * 5, 3);
        // Optionally, also create a console sink.
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();

        // Optionally set individual sink levels if you need to filter or segregate messages.
        basic_sink->set_level(spdlog::level::info);
        rotating_sink->set_level(spdlog::level::debug);
        console_sink->set_level(spdlog::level::trace);

        // Add the sinks to a vector.
        std::vector<spdlog::sink_ptr> sinks{ basic_sink, rotating_sink, console_sink };

        // Create the multi-sink logger.
        logger = std::make_shared<spdlog::logger>("multi_sink_logger", sinks.begin(), sinks.end());
        // Register the logger so that it can be retrieved later, if desired.
        spdlog::register_logger(logger);

        // Set the overall log level and flush behavior.
        logger->set_level(spdlog::level::trace);
        logger->flush_on(spdlog::level::trace);
    }
    catch (const spdlog::spdlog_ex& ex) {
        std::cerr << "Log initialization failed: " << ex.what() << std::endl;
        throw;
    }
}

std::shared_ptr<spdlog::logger> Logger::getLogger() {
    return logger;
}