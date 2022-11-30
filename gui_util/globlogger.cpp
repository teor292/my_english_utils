#include "globlogger.h"
#include <filesystem>
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/stdout_sinks.h"

namespace fs = std::filesystem;

namespace
{
    const auto LOG_DIR = "log";
    const auto LOG_FILE = "log.txt";
}

void GlobLogger::Initialize()
{
    fs::path pth{LOG_DIR};
    pth /= LOG_FILE;


    spdlog::default_logger_raw()->set_level(spdlog::level::level_enum::trace);
    spdlog::default_logger_raw()->sinks().clear();

    auto file = std::make_shared<spdlog::sinks::rotating_file_sink_st>(pth.string(),
                                                                       500000, 1);
    file->set_level(spdlog::level::level_enum::trace);

    auto console = std::make_shared<spdlog::sinks::stdout_sink_st>();
    console->set_level(spdlog::level::level_enum::trace);

    spdlog::default_logger_raw()->sinks().push_back(file);
    spdlog::default_logger_raw()->sinks().push_back(console);
}
