

#ifndef SOLARSYSTEM_CONSOLEHANDLER_H
#define SOLARSYSTEM_CONSOLEHANDLER_H

#include <string>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>
#include <windows.h>
#include <thread>


namespace SolarSystem
{
    namespace Core
    {
        #define CON_DECLARE CConsoleHandler mConsoleHandler;
        #define CON_INIT(debugStream, errorStream) mConsoleHandler(debugStream, errorStream);

        #define LOG(message) mDebugWriter.log((std::stringstream{} << message).str());
        #define LOGL(LEVEL, message) mDebugWriter.log(LEVEL, (std::stringstream{} << message).str());

        #define LOG_POINT mDebugWriter.log((std::stringstream{} << __FILE__ << ":" << __LINE__).str());

        enum eLogLevel
        {
            LOG_LEVEL_DEBUG,
            LOG_LEVEL_ERROR
        };


        class CConsoleHandler
        {
        private:
            std::ostream& mDebugStream;
            std::ostream& mErrorStream;

            bool mIsRunning;
            HANDLE hCommandPipe;
            HANDLE hResponsePipe;
            std::thread mCommandThread;

        public:

            CConsoleHandler();

            CConsoleHandler(std::ostream& debugStream, std::ostream& errorStream);

            void start();

            void start(bool shouldStart);

            void stop();

            void log(eLogLevel logLevel, const std::string& message) const;

            void log(const std::string& message) const;

        private:

            void command_handler();

            std::ostream& get_ostream_from_level(eLogLevel logLevel) const;

            static std::string get_current_datetime();

        };

    }
}

#endif //SOLARSYSTEM_CONSOLEHANDLER_H
