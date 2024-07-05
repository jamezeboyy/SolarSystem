

#include <sstream>
#include <deque>
#include <algorithm>
#include "command_handler.h"

namespace SolarSystem
{
    namespace Core
    {

        CCommandHandler::CCommandHandler()
        {
            mHandlerMap = {
                    {"EXIT", std::bind(&CCommandHandler::exit_handler, this, std::placeholders::_1)},
                    {"HELP", std::bind(&CCommandHandler::help_handler, this, std::placeholders::_1)}
            };
        }

        std::string CCommandHandler::handle(const std::string& command)
        {
            std::string s;
            std::stringstream ss(command);

            std::deque<std::string> parsedInput;

            while (getline(ss, s, ' '))
                parsedInput.push_back(s);

            if (parsedInput.empty()) return "Failed: Command string is empty!";

            std::string cmd = parsedInput.front();
            parsedInput.pop_front();
            std::transform(cmd.begin(), cmd.end(), cmd.begin(), std::toupper);

            if (mHandlerMap.count(cmd) == 0) return "Unknown command!";

            auto handlerFunc = mHandlerMap[cmd];

            // TODO maybe more efficient way for this?
            std::vector<std::string> params(parsedInput.begin(), parsedInput.end());
            return handlerFunc(params);
        }

        void CCommandHandler::add_callback(eCommands command, CCommandCallbacker* callbacker)
        {
            mCommandCallbackMap[command] = callbacker;
        }

        std::string CCommandHandler::exit_handler(const std::vector<std::string>& parameters)
        {
            mCommandCallbackMap[eCommands::EXIT]->callback_handler(parameters);
            return "Debugger connection closed...";
        }


        std::string CCommandHandler::help_handler(const std::vector<std::string>& parameters)
        {
            std::string help = "Debugger commands:\n";
            for (const auto& pair : mHandlerMap) {
                help += " - " + pair.first + "\n";
            }
            return help;
        }

    }

}