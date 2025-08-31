#pragma once
#include "Types_Compiler.hpp"

namespace IStudio::Log
{
    enum class LogLevel
    {
        DEBUG,
        INFO,
        WARNING,
        ERROR,
        CRITICAL,
        TRACE,
        VERBOSE
    };

    class Logger
    {
    public:
        Logger()
            : logFile(nullptr), logLevel(LogLevel::DEBUG), logDepth(1),
              defaultLogDepth(0), enabled(true),
              logMessageStream(std::make_shared<std::stringstream>())
        {}

        Logger(const std::string &filename, LogLevel level = LogLevel::DEBUG, int depth = 0, int defaultDepth = 0)
            : logFileName(filename), logLevel(level), logDepth(depth),
              defaultLogDepth(defaultDepth), enabled(true),
              logMessageStream(std::make_shared<std::stringstream>())
        {
            logFile = std::make_shared<std::ofstream>(filename, std::ios::app);
        }

        Logger(const Logger &other)
        {
            logFile = nullptr;
            copyFrom(other);
        }

        Logger &operator=(const Logger &other)
        {
            if (this != &other)
            {
                if (logFile && logFile->is_open())
                    logFile->close();
                logFile = nullptr;
                copyFrom(other);
            }
            return *this;
        }

        ~Logger()
        {
            if (logFile && logFile->is_open())
            {
                logFile = nullptr;
            }
        }

        // Scoped logging wrapper
        template <typename T>
        class LogWrapper
        {
        public:
            LogWrapper(T &logger, LogLevel level, int depth)
                : logger(logger),
                  prevLogLevel(logger.logLevel),
                  prevLogDepth(logger.logDepth)
            {
                logger.logLevel = level;
                logger.logDepth = depth;
            }

            template <typename U>
            LogWrapper &operator<<(const U &value)
            {
                *logger.logMessageStream << value;
                return *this;
            }

            ~LogWrapper()
            {
                logger.log(logger.logMessageStream->str());
                logger.logMessageStream->str("");
                logger.logLevel = prevLogLevel;
                logger.logDepth = prevLogDepth;
            }

        private:
            T &logger;
            LogLevel prevLogLevel;
            int prevLogDepth;
        };

        // const-correct operator() with internal mutable state
        LogWrapper<Logger> operator()(LogLevel level, int depth = 0) const
        {
            return LogWrapper<Logger>(const_cast<Logger &>(*this), level, depth);
        }

        void setDefaultDepth(int depth) { defaultLogDepth = depth; }
        void setDefaultLogLevel(const std::vector<LogLevel> &levels) { defaultLogLevel = levels; }

        void enable() { enabled = true; }
        void disable() { enabled = false; }
        bool isEnabled() const { return enabled; }

        LogLevel getLogLevel() const { return logLevel; }
        int getLogDepth() const { return logDepth; }

        void setMaxFileSize(size_t bytes) { maxFileSize = bytes; }
        void setMaxBackupFiles(int count) { maxBackupFiles = count; }

    private:
        std::streamsize getFileSize()
        {
            if (!logFile || !logFile->is_open()) return 0;
            logFile->flush();
            std::ifstream in(logFileName, std::ifstream::ate | std::ifstream::binary);
            return in.tellg();
        }

        void rotateLogFile()
        {
            if (logFile) logFile->close();

            std::string lastBackup = logFileName + "." + std::to_string(maxBackupFiles);
            std::remove(lastBackup.c_str());

            for (int i = maxBackupFiles - 1; i >= 1; --i)
            {
                std::string oldName = logFileName + "." + std::to_string(i);
                std::string newName = logFileName + "." + std::to_string(i + 1);
                std::rename(oldName.c_str(), newName.c_str());
            }

            std::string newName = logFileName + ".1";
            std::rename(logFileName.c_str(), newName.c_str());

            logFile = std::make_shared<std::ofstream>(logFileName, std::ios::trunc);
        }

        void copyFrom(const Logger &other)
        {
            logLevel = other.logLevel;
            logDepth = other.logDepth;
            defaultLogDepth = other.defaultLogDepth;
            enabled = other.enabled;
            defaultLogLevel = other.defaultLogLevel;
            logFileName = other.logFileName;
            logFile = other.logFile;
            logMessageStream = other.logMessageStream;
        }

        void log(const std::string &message)
        {
            if (!enabled || logDepth > defaultLogDepth || !isLogLevelDefault())
                return;

            std::time_t now = std::time(nullptr);
            char timeBuffer[20];
            std::strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S", std::localtime(&now));

            std::string logMessage = "[" + std::string(timeBuffer) + "] ";

            switch (logLevel)
            {
            case LogLevel::DEBUG: logMessage += "[DEBUG] "; break;
            case LogLevel::INFO: logMessage += "[INFO] "; break;
            case LogLevel::WARNING: logMessage += "[WARNING] "; break;
            case LogLevel::ERROR: logMessage += "[ERROR] "; break;
            case LogLevel::CRITICAL: logMessage += "[CRITICAL] "; break;
            case LogLevel::TRACE: logMessage += "[TRACE] "; break;
            case LogLevel::VERBOSE: logMessage += "[VERBOSE] "; break;
            }

            logMessage += message;

            if (logFile && logFile->is_open())
            {
                if (getFileSize() > static_cast<std::streamsize>(maxFileSize))
                {
                    rotateLogFile();
                }

                *logFile << logMessage << std::endl;
                logFile->flush();
            }
            else
            {
                std::cout << logMessage << std::endl;
            }
        }

        bool isLogLevelDefault() const
        {
            if (defaultLogLevel.empty()) return true;
            for (const auto &lvl : defaultLogLevel)
                if (lvl == logLevel) return true;
            return false;
        }

        std::string logFileName;
        std::shared_ptr<std::ofstream> logFile;

        // Marked mutable to allow logging from const contexts
        mutable std::shared_ptr<std::stringstream> logMessageStream;
        mutable LogLevel logLevel;
        mutable int logDepth;

        int defaultLogDepth;
        std::vector<LogLevel> defaultLogLevel;
        bool enabled;
        size_t maxFileSize = 1024 * 100; // 100 KB
        int maxBackupFiles = 3;
    };
}
