#include "log.h"


namespace LR
{
    Logger::Logger(LogLevel level) : level_(level)
    {
    }

    void Logger::Write(LogLevel level, const char *format, ...)
    {
        va_list val;
        va_start(val, format);
        WriteImpl(level, format, &val);
        va_end(val);
    }

    inline void Logger::WriteImpl(LogLevel level, const char *format, va_list *val)
    {

        std::string level_str = GetLevelStr(level);
        std::string time_str = GetSystemTime();
        va_list val_copy;
        va_copy(val_copy, *val);
        printf("[%s] [%s] ", level_str.c_str(), time_str.c_str());
        vprintf(format, *val);
        fflush(stdout);
        va_end(val_copy);
    }

    std::string Logger::GetSystemTime()
    {
        time_t t = time(0);
        char str[64];
        strftime(str, sizeof(str), "%Y-%m-%d %H:%M:%S", localtime(&t));
        return str;
    }

    std::string Logger::GetLevelStr(LogLevel level)
    {
        switch (level)
        {
        case LogLevel::Debug:
            return "DEBUG";
        case LogLevel::Info:
            return "INFO";
        case LogLevel::Error:
            return "ERROR";
        case LogLevel::Fatal:
            return "FATAL";
        default:
            return "UNKNOW";
        }
    }

    Logger Log::logger_;

    void Log::ResetLogLevel(LogLevel level)
    {
        logger_.ResetLogLevel(level);
    }

    void Log::Write(LogLevel level, const char *format, ...)
    {
        va_list val;
        va_start(val, format);
        logger_.WriteImpl(level, format, &val);
        va_end(val);
    }

    void Log::Info(const char *format, ...)
    {
        va_list val;
        va_start(val, format);
        logger_.WriteImpl(LogLevel::Info, format, &val);
        va_end(val);
    }

    void Log::Error(const char *format, ...)
    {
        va_list val;
        va_start(val, format);
        logger_.WriteImpl(LogLevel::Error, format, &val);
        va_end(val);
    }

    void Log::Fatal(const char *format, ...)
    {
        va_list val;
        va_start(val, format);
        logger_.WriteImpl(LogLevel::Fatal, format, &val);
        va_end(val);
    }


    void Log::Debug(const char *format, ...)
    {
        va_list val;
        va_start(val, format);
        logger_.WriteImpl(LogLevel::Debug, format, &val);
        va_end(val);
    }
}
