

#include "console_handler.h"

#include <thread>
#include <mutex>
#include <regex>

namespace SolarSystem
{
    namespace Core
    {

        const char* COMMAND_PIPE_NAME = R"(\\.\pipe\SolarSystemDebugCommandPipe)";
        const char* RESPONSE_PIPE_NAME = R"(\\.\pipe\SolarSystemDebugResponsePipe)";

        std::mutex mtxLog;

        CConsoleHandler::CConsoleHandler() :
                mDebugStream(&std::cout),
                mErrorStream(&std::cerr),
                mIsRunning(false),
                hCommandPipe(nullptr),
                hResponsePipe(nullptr),
                mShouldLogThread(false),
                mShouldLogLevel(false),
                mLogFilter("NONE")


        {
//            mSetterCommandHandler.add_callback(eCommandsSetter::OBJ1, <obj class here>);
            mTLCommandHandler.add_callback(eCommands::EXIT, this);
            mTLCommandHandler.add_callback(eCommands::LOG, this);
            mTLCommandHandler.add_callback(eCommands::SET, &mSetterCommandHandler);
        }

        CConsoleHandler::~CConsoleHandler() = default;


        void CConsoleHandler::start()
        {
            start(false);
        }

        void CConsoleHandler::start(bool startConsole)
        {
            mIsRunning = true;

            mStartThread = std::thread([this, startConsole]
            {
               SECURITY_DESCRIPTOR sd;
               InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);
               SetSecurityDescriptorDacl(&sd, TRUE, nullptr, FALSE);

               SECURITY_ATTRIBUTES sa;
               sa.nLength = sizeof(SECURITY_ATTRIBUTES);
               sa.lpSecurityDescriptor = &sd;
               sa.bInheritHandle = FALSE;

               hCommandPipe = CreateNamedPipe(
                       COMMAND_PIPE_NAME,
                       PIPE_ACCESS_DUPLEX,
                       PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
                       1,
                       1024,
                       1024,
                       0,
                       &sa
               );

               hResponsePipe = CreateNamedPipe(
                       RESPONSE_PIPE_NAME,
                       PIPE_ACCESS_DUPLEX,
                       PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
                       1,
                       1024,
                       1024,
                       0,
                       &sa
               );

               if (hCommandPipe == INVALID_HANDLE_VALUE || hResponsePipe == INVALID_HANDLE_VALUE)
               {
                   log(LOG_LEVEL_ERROR, "Failed to create pipes.");
                   return;
               }

               log("Opened connection. Waiting for debug command shell to connect...");

               if (startConsole)
               {
                   STARTUPINFO si;
                   PROCESS_INFORMATION pi;

                   ZeroMemory(&si, sizeof(si));
                   si.cb = sizeof(si);
                   si.dwFlags = STARTF_USESHOWWINDOW;
                   si.wShowWindow = SW_SHOW;

                   ZeroMemory(&pi, sizeof(pi));

                   log("Starting debug shell process...");
                   const char *cmd = "cmd.exe /C start SolarSystemDebugShell.exe";

                   if (!CreateProcess(nullptr,
                                      const_cast<char *>(cmd),
                                      nullptr,
                                      nullptr,
                                      FALSE,
                                      CREATE_NEW_CONSOLE,
                                      nullptr,
                                      nullptr,
                                      &si,
                                      &pi))
                   {
                       log(LOG_LEVEL_ERROR,
                           "Failed to create terminal: " + std::to_string(GetLastError()));
                       return;
                   }

                   CloseHandle(pi.hProcess);
                   CloseHandle(pi.hThread);
               }


               BOOL connectedCommand = ConnectNamedPipe(hCommandPipe, nullptr) ? TRUE : (
                       GetLastError() == ERROR_PIPE_CONNECTED);
               BOOL connectedResponse = ConnectNamedPipe(hResponsePipe, nullptr) ? TRUE : (
                       GetLastError() == ERROR_PIPE_CONNECTED);

               if (!connectedCommand || !connectedResponse)
               {
                   log(LOG_LEVEL_ERROR,
                       "Connecting to debugger failed: " + std::to_string(GetLastError()));
                   CloseHandle(hCommandPipe);
                   CloseHandle(hResponsePipe);
                   return;
               }

               log("Debug command shell connected. Now Handling commands...");
            });

            mCommandThread = std::thread(&CConsoleHandler::command_handler, this);
        }

        void CConsoleHandler::stop()
        {
            if(!mIsRunning) return;

            mIsRunning = false;

            if (mStartThread.joinable())
                mStartThread.join();
            else if (mCommandThread.joinable())
                mCommandThread.join();

            CloseHandle(hCommandPipe);
            CloseHandle(hResponsePipe);
        }

        void CConsoleHandler::command_handler()
        {
            char buffer[1024];
            DWORD bytesRead;

            if (mStartThread.joinable())
                mStartThread.join();

            while (mIsRunning)
            {
                bool result = ReadFile(hCommandPipe, buffer, sizeof(buffer) - 1, &bytesRead, nullptr);
                if (result)
                {
                    if (bytesRead > 0)
                    {
                        buffer[bytesRead] = '\0';
                        std::string command(buffer);

                        log("Received command: " + command);
                        std::string response = mTLCommandHandler.handle(command);
                        DWORD bytesWritten;
                        WriteFile(hResponsePipe, response.c_str(), response.length(), &bytesWritten, nullptr);

                    }
                    else
                    {
                        log(LOG_LEVEL_ERROR, "Debug connection closed by client.");
                        break;
                    }
                }
                else
                {
                    DWORD error = GetLastError();
                    if (error == ERROR_BROKEN_PIPE)
                    {
                        log(LOG_LEVEL_ERROR, "Debug connection closed by client.");
                    }
                    else
                    {
                        log(LOG_LEVEL_ERROR, "Error reading from command pipe. Error code: " + std::to_string(error));
                    }
                    break;
                }
            }
            // TODO make this reopen pipe, complicated give we can NOT start while in this thread..
            // TODO Need to poll or notify in someway requires addition thread just for restarting, seems overkill idk...
        }

        void CConsoleHandler::log(eLogLevel logLevel, const std::string& message) const
        {
            std::unique_lock<std::mutex> lock(mtxLog);
            std::ostream* stream = get_ostream_from_level(logLevel);


            if (mLogFilter != "NONE")
            {
                std::regex filterRegex(mLogFilter);
                if (!std::regex_search(message, filterRegex))
                    return;
            }


            if (mShouldLogLevel)
            {
                std::string level;
                switch (logLevel)
                {
                    case eLogLevel::LOG_LEVEL_DEBUG:
                        level = "DEBUG";
                        break;
                    case eLogLevel::LOG_LEVEL_ERROR:
                        level = "ERROR";
                        break;
                    default:
                        level = "UNKNOWN";
                        break;
                }
                (*stream) << "[" << level << "] ";
            }

            if (mShouldLogThread)
                (*stream) << "(" <<  std::this_thread::get_id() << ") ";

            (*stream) << get_current_datetime() << " | " << message << std::endl;

        }

        void CConsoleHandler::log(const std::string& message) const
        {
            log(eLogLevel::LOG_LEVEL_DEBUG, message);
        }

        std::string CConsoleHandler::callback_handler(std::string cmd, std::vector<std::string> parameters)
        {
            if(cmd == "EXIT")
            {
                log("Closing down debugger command connection...");
                mIsRunning = false;

            }
            else if(cmd == "LOG")
            {
                if (parameters.empty())
                {
                    return (std::stringstream{} << "Log settings | Thread: " << mShouldLogThread
                                                << ", Level: " << mShouldLogLevel
                                                << ", Filter: " << mLogFilter).str();
                }
                else
                {
                    std::string type = parameters[0];
                    if (type == "thread")
                    {
                        if (parameters.size() > 1)
                        {
                            mShouldLogThread = (parameters[1] == "true" || parameters[1] == "1");
                        }
                        return std::to_string(mShouldLogThread);
                    }
                    else if(type == "level")
                    {
                        if (parameters.size() > 1)
                        {
                            mShouldLogLevel = (parameters[1] == "true" || parameters[1] == "1");
                        }
                        return std::to_string(mShouldLogLevel);
                    }
                    else if(type == "filter")
                    {
                        if (parameters.size() > 1)
                        {
                            mLogFilter = parameters[1];
                        }
                        return mLogFilter;
                    }
                    else
                    {
                        return "Unknown log parameters!";
                    }
                }
            }
            return "Unknown parameters!";

        }

        std::ostream* CConsoleHandler::get_ostream_from_level(eLogLevel logLevel) const
        {
            switch (logLevel)
            {
                case eLogLevel::LOG_LEVEL_DEBUG:
                    return mDebugStream;
                case eLogLevel::LOG_LEVEL_ERROR:
                    return mErrorStream;
                default:
                    return mDebugStream;
            }
        }



        std::string CConsoleHandler::get_current_datetime()
        {
            auto now = std::chrono::system_clock::now();

            std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);

            auto duration = now.time_since_epoch();
            auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration) % 1000;

            std::tm now_tm{};

            localtime_s(&now_tm, &now_time_t);
            std::stringstream ss;
            ss << std::put_time(&now_tm, "%Y-%m-%d.%X")
               << '.' << std::setfill('0')
               << std::setw(3) << millis.count();
            return ss.str();
        }

        void CConsoleHandler::register_set_callback(eCommandsSetter command, CCommandCallbacker* callbacker)
        {
            mSetterCommandHandler.add_callback(command, callbacker);
        }

        void CConsoleHandler::register_tl_callback(eCommands command, CCommandCallbacker* callbacker)
        {
            mTLCommandHandler.add_callback(command, callbacker);
        }

        void CConsoleHandler::set_debug_stream(std::ostream& debugStream)
        {
            mDebugStream = &debugStream;
        }

        void CConsoleHandler::set_error_stream(std::ostream& errorStream)
        {
            mErrorStream = &errorStream;
        }

        CConsoleHandler& CConsoleHandler::get_instance()
        {
            static CConsoleHandler instance;
            return instance;
        }

    }
}