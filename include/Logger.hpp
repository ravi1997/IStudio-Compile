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
        Logger() : logFile(nullptr), logLevel(LogLevel::DEBUG), logDepth(1), defaultLogDepth(0), enabled(true) {}

        Logger(const std::string &filename, LogLevel level = LogLevel::DEBUG, int depth = 0, int defaultDepth = 0)
            : logLevel(level), logDepth(depth), defaultLogDepth(defaultDepth), enabled(true)
        {
            logFile = std::make_unique<std::ofstream>(filename, std::ios::app);
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
                if (logFile != nullptr && logFile->is_open())
                    logFile->close();
                logFile = nullptr;
                copyFrom(other);
            }
            return *this;
        }

        ~Logger()
        {
            if (logFile != nullptr && logFile->is_open())
                logFile->close();
        }

        template <typename T>
        class LogWrapper
        {
        public:
            LogWrapper(Logger &logger, LogLevel level, int depth) : logger(logger), prevLogLevel(logger.logLevel), prevLogDepth(logger.logDepth)
            {
                logger.logLevel = level;
                logger.logDepth = depth;
            }

            template <typename U>
            LogWrapper &operator<<(const U &value)
            {
                logger.logMessageStream << value;
                return *this;
            }

            ~LogWrapper()
            {
                logger.log(logger.logMessageStream.str());
                logger.logMessageStream.str("");
                logger.logLevel = prevLogLevel;
                logger.logDepth = prevLogDepth;
            }

        private:
            Logger &logger;
            LogLevel prevLogLevel;
            int prevLogDepth;
        };

        LogWrapper<Logger> operator()(LogLevel level, int depth = 0)
        {
            return LogWrapper<Logger>(*this, level, depth);
        }

        void setDefaultDepth(int depth)
        {
            defaultLogDepth = depth;
        }

        void setDefaultLogLevel(const std::vector<LogLevel> &levels)
        {
            defaultLogLevel = levels;
        }

        void enable()
        {
            enabled = true;
        }

        void disable()
        {
            enabled = false;
        }

        bool isEnabled() const
        {
            return enabled;
        }

        LogLevel getLogLevel() const
        {
            return logLevel;
        }

        int getLogDepth() const
        {
            return logDepth;
        }

    private:
        void copyFrom(const Logger &other)
        {
            logLevel = other.logLevel;
            logDepth = other.logDepth;
            defaultLogDepth = other.defaultLogDepth;
            enabled = other.enabled;
            defaultLogLevel = other.defaultLogLevel;

            if (other.logFile != nullptr && other.logFile->is_open())
            {
                logFile = std::make_unique<std::ofstream>(*other.logFile);
            }

            logMessageStream.str(other.logMessageStream.str());
        }

        void log(const std::string &message)
        {
            if (!isEnabled || logDepth > defaultLogDepth || !isLogLevelDefault())
                return;

            std::time_t now = std::time(nullptr);
            char timeBuffer[20];
            std::strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S", std::localtime(&now));

            std::string logMessage = "[" + std::string(timeBuffer) + "] ";

            switch (logLevel)
            {
            case LogLevel::DEBUG:
                logMessage += "[DEBUG] ";
                break;
            case LogLevel::INFO:
                logMessage += "[INFO] ";
                break;
            case LogLevel::WARNING:
                logMessage += "[WARNING] ";
                break;
            case LogLevel::ERROR:
                logMessage += "[ERROR] ";
                break;
            case LogLevel::CRITICAL:
                logMessage += "[CRITICAL] ";
                break;
            }

            logMessage += message;

            if (logFile != nullptr && logFile->is_open())
            {
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
            if (defaultLogLevel.empty())
            {
                return true; // Default level is every level
            }
            for (const LogLevel &level : defaultLogLevel)
            {
                if (level == logLevel)
                {
                    return true;
                }
            }
            return false;
        }

        std::unique_ptr<std::ofstream> logFile;
        std::stringstream logMessageStream;
        LogLevel logLevel;
        int logDepth;
        int defaultLogDepth;
        std::vector<LogLevel> defaultLogLevel;
        bool enabled;
    };
} // namespace IStudio::Log
