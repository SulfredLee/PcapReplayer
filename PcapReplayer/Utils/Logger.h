/*
 * This Logger can save log to files, cah have different log level, can using << operator thread safe
 * Very interesting implementation, active lock in constructor, using move() to keep the lock alive between different Logger object
 */

#ifndef LOGGER_H
#define LOGGER_H
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <map>
#include <thread>
#include <mutex>
#include <memory>

#include <unistd.h>
#include <sys/syscall.h>
#include <sys/time.h>

#include <ThreadPool.h>

class Logger
{
 public:
    enum class LogLevel { DEBUG, WARN, INFO, ERR };
    struct LoggerConfig
    {
    public:
        LoggerConfig()
        {
            logLevel = Logger::LogLevel::WARN;
            logPath = ".";
            fileSize = 0;
            fileSizeLimit = 4 * 1024 * 1024; // 4 MByte
            isToConsole = true;
            isToFile = false;
        };
        ~LoggerConfig() {};
    public:
        Logger::LogLevel logLevel;
        std::string logPath;
        std::string logFilePathName;
        int fileSize;
        int fileSizeLimit;
        bool isToConsole;
        bool isToFile;
    };
 public:
    ~Logger();

    static Logger& GetInstance();
    static std::string GetFileName(const std::string& fillPath);

    void InitComponent(const LoggerConfig& config);
    LoggerConfig GetConfig();
    void Log(LogLevel logLevel, const char* format, ...);
    void Log(LogLevel logLevel, const std::string& logMsg);
    void AddClassName(std::string className, void* object);
    std::string GetClassName(void* object, std::string prettyFunction);
 private:
    LoggerConfig m_config;
    std::ofstream m_outFH;
    std::mutex m_mutex;
    LogLevel m_oneTimeLevel;
    std::map<void*, std::string> m_classNameMap; // key: object address, object class name
    ThreadPool<std::shared_ptr<std::string> > m_logWorker;
 private:
    Logger();
    Logger(const Logger&) = delete;
    Logger(const Logger&&) = delete;
    Logger& operator=(const Logger&) = delete;
    Logger& operator=(const Logger&&) = delete;

    std::string GetCurrentTime();
    std::string GetLogLevelName(LogLevel logLevel);
    std::string _GetClassName(const std::string& prettyFunction);
    bool OpenLogFile();
    void LogImplement(char dest[], int size, LogLevel logLevel, bool useTimeStamp, bool printLogLocation, void *logObject);
    void PrintLog(const std::shared_ptr<std::string>& logStr);
};

class StreamLogger
{
 public:
    // static std::mutex staticOutLock; // the original usage is the activate this lock in the constructor

    std::unique_lock<std::mutex> m_lock;
    std::shared_ptr<std::stringstream> m_stream; // can't make this reference so we can move
    Logger::LogLevel m_level;
 public:
    StreamLogger(std::shared_ptr<std::stringstream> stream, const Logger::LogLevel& logLevel, void *logObject, const std::string& functionName, const int& lineNumber)
        : m_stream(std::move(stream))
        // : m_lock(staticOutLock)
        // , m_stream(&stream)
    {
        m_stream->str("");
        m_level = logLevel;
        if (logObject)
            *m_stream << std::setw(25) << std::setfill(' ') << Logger::GetInstance().GetClassName(logObject, __PRETTY_FUNCTION__) << "::";
        *m_stream << std::setw(25) << std::setfill(' ') << functionName << ":"
                  << std::setw(6) << std::setfill(' ') << lineNumber << ","
                  << std::setw(6) << std::setfill(' ') << syscall(SYS_gettid) << ",";
    }
    ~StreamLogger()
    {
        Logger::GetInstance().Log(m_level, m_stream->str());
    }

    StreamLogger(StreamLogger&& other)
        : m_stream(std::move(other.m_stream))
        // : m_lock(std::move(other.m_lock))
        // , m_stream(other.m_stream)
    {
        // other.m_stream = nullptr;
    }

    friend StreamLogger&& operator << (StreamLogger&& s, std::stringstream& (*arg)(std::stringstream&))
    {
        (*s.m_stream) << arg;
        return std::move(s);
    }

    friend StreamLogger&& operator << (StreamLogger&& s, std::ostream& (*arg)(std::ostream&))
    {
        (*s.m_stream) << arg;
        return std::move(s);
    }

    template <typename Arg>
        friend StreamLogger&& operator << (StreamLogger&& s, Arg&& arg)
    {
        (*s.m_stream) << std::forward<Arg>(arg);
        return std::move(s);
    }
};

StreamLogger CreateStreamLogger(const Logger::LogLevel& logLevel, void *logObject, const std::string& functionName, const int& lineNumber);

#define __LOGMSG(level, ...)                                            \
    do{                                                                 \
        char buffer[5120];                                              \
        snprintf(buffer, sizeof(buffer), __VA_ARGS__);                  \
        Logger::GetInstance().Log(level, "%25s::%25s:%6d,%6d,%s", Logger::GetInstance().GetClassName(this, __PRETTY_FUNCTION__).c_str(), __FUNCTION__, __LINE__, syscall(SYS_gettid), buffer); \
    }while(0)

#define __LOGMSG_C(level, ...)                                          \
    do{                                                                 \
        char buffer[5120];                                              \
        snprintf(buffer, sizeof(buffer), __VA_ARGS__);                  \
        Logger::GetInstance().Log(level, "%25s:%6d,%6d,%s", __FUNCTION__, __LINE__, syscall(SYS_gettid), buffer); \
    }while(0)

#define LOGMSG_DBG(...) __LOGMSG(Logger::LogLevel::DEBUG, __VA_ARGS__)
#define LOGMSG_MSG(...)  __LOGMSG(Logger::LogLevel::INFO, __VA_ARGS__)
#define LOGMSG_WRN(...)  __LOGMSG(Logger::LogLevel::WARN, __VA_ARGS__)
#define LOGMSG_ERR(...) __LOGMSG(Logger::LogLevel::ERR, __VA_ARGS__)
#define LOGMSG_DBG_C(...) __LOGMSG_C(Logger::LogLevel::DEBUG, __VA_ARGS__)
#define LOGMSG_MSG_C(...)  __LOGMSG_C(Logger::LogLevel::INFO, __VA_ARGS__)
#define LOGMSG_WRN_C(...)  __LOGMSG_C(Logger::LogLevel::WARN, __VA_ARGS__)
#define LOGMSG_ERR_C(...) __LOGMSG_C(Logger::LogLevel::ERR, __VA_ARGS__)
#define LOGMSG_DBG_S() CreateStreamLogger(Logger::LogLevel::DEBUG, this, __FUNCTION__, __LINE__)
#define LOGMSG_MSG_S() CreateStreamLogger(Logger::LogLevel::INFO, this, __FUNCTION__, __LINE__)
#define LOGMSG_WRN_S() CreateStreamLogger(Logger::LogLevel::WARN, this, __FUNCTION__, __LINE__)
#define LOGMSG_ERR_S() CreateStreamLogger(Logger::LogLevel::ERR, this, __FUNCTION__, __LINE__)
#define LOGMSG_DBG_S_C() CreateStreamLogger(Logger::LogLevel::DEBUG, NULL, __FUNCTION__, __LINE__)
#define LOGMSG_MSG_S_C() CreateStreamLogger(Logger::LogLevel::INFO, NULL, __FUNCTION__, __LINE__)
#define LOGMSG_WRN_S_C() CreateStreamLogger(Logger::LogLevel::WARN, NULL, __FUNCTION__, __LINE__)
#define LOGMSG_ERR_S_C() CreateStreamLogger(Logger::LogLevel::ERR, NULL, __FUNCTION__, __LINE__)
#define LOGMSG_CLASS_NAME(className) Logger::GetInstance().AddClassName(className, this)
#define LOGMSG_INIT(config) Logger::GetInstance().InitComponent(config)

#endif
