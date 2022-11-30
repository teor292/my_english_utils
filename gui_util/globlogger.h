#ifndef GLOBLOGGER_H
#define GLOBLOGGER_H

#include "spdlog/spdlog.h"


class GlobLogger
{
public:
    static void Initialize();

    inline static std::shared_ptr<spdlog::logger> GetLogger()
    {
        return _logger;
    }
};

#endif // GLOBLOGGER_H
