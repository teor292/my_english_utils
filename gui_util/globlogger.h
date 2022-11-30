#ifndef GLOBLOGGER_H
#define GLOBLOGGER_H


#include "spdlog/spdlog.h"

#define LOGT(...) SPDLOG_TRACE(__VA_ARGS__)
#define LOGD(...) SPDLOG_DEBUG(__VA_ARGS__)
#define LOGI(...) SPDLOG_INFO(__VA_ARGS__)
#define LOGW(...) SPDLOG_WARN(__VA_ARGS__)
#define LOGE(...) SPDLOG_ERROR(__VA_ARGS__)

class GlobLogger
{
public:
    static void Initialize();

};

#endif // GLOBLOGGER_H
