#ifndef LOG_H
#define LOG_H
#include <fstream>
#include <string>

namespace LR
{

    enum class LogLevel : int
    {
        Debug = 0,
        Info = 1,
        Error = 2,
        Fatal = 3
    };

    class Logger
    {
        friend class Log;

    public:
        explicit Logger(LogLevel level = LogLevel::Info);
        void ResetLogLevel(LogLevel level)
        {
            level_ = level;
        }

        void Write(LogLevel level, const char *format, ...);
        void Debug(const char *format, ...);
        void Info(const char *format, ...);
        void Error(const char *format, ...);
        void Fatal(const char *format, ...);

    private:
        void WriteImpl(LogLevel level, const char *format, va_list *val);
        std::string GetSystemTime();
        std::string GetLevelStr(LogLevel level);
        LogLevel level_;
        //    bool is_kill_fatal_;
        Logger(const Logger &);
        void operator=(const Logger &);
    };

    class Log
    {
    public:
        static void ResetLogLevel(LogLevel level);
        static void Write(LogLevel level, const char *format, ...);
        static void Debug(const char *format, ...);
        static void Info(const char *format, ...);
        static void Error(const char *format, ...);
        static void Fatal(const char *format, ...);
    private:
        static Logger logger_;
    };

}


#endif