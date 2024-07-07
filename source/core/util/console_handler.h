

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
#include "tlcommand_handler.h"
#include "settercommand_handler.h"



namespace SolarSystem
{
    namespace Core
    {

        #define LOG(message) CConsoleHandler::get_instance().log((std::stringstream{} << message).str());
        #define LOGL(LEVEL, message) CConsoleHandler::get_instance().log(LEVEL, (std::stringstream{} << message).str());

        #define LOG_POINT CConsoleHandler::get_instance().log((std::stringstream{} << __FILE__ << ":" << __LINE__).str());

        enum eLogLevel
        {
            LOG_LEVEL_DEBUG,
            LOG_LEVEL_ERROR
        };


        class CConsoleHandler : public CCommandCallbacker
        {
        private:
            std::ostream* mDebugStream;
            std::ostream* mErrorStream;
            CTLCommandHandler mTLCommandHandler;
            CSetterCommandHandler mSetterCommandHandler;

            bool mIsRunning;
            HANDLE hCommandPipe;
            HANDLE hResponsePipe;
            std::thread mCommandThread;
            std::thread mStartThread;

            bool mShouldLogThread;
            bool mShouldLogLevel;
            std::string mLogFilter;



        public:

            ~CConsoleHandler();

            void start();

            void start(bool shouldStart);

            void stop();

            void log(eLogLevel logLevel, const std::string& message) const;

            void log(const std::string& message) const;

            std::string callback_handler(std::string cmd, std::vector<std::string> parameters) override;

            void register_set_callback(eCommandsSetter command, CCommandCallbacker* callbacker);

            void register_tl_callback(eCommands command, CCommandCallbacker* callbacker);

            void set_debug_stream(std::ostream& debugStream);

            void set_error_stream(std::ostream& errorStream);

            static CConsoleHandler& get_instance();

            CConsoleHandler(CConsoleHandler& inst) = delete;

            void operator=(const CConsoleHandler& inst) = delete;

        private:

            CConsoleHandler();

            void command_handler();

            std::ostream* get_ostream_from_level(eLogLevel logLevel) const;

            static std::string get_current_datetime();

        };

    }
}

#endif //SOLARSYSTEM_CONSOLEHANDLER_H
