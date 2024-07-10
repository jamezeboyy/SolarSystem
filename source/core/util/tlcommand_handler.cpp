

#include "tlcommand_handler.h"

namespace SolarSystem
{
    namespace Core
    {

        CTLCommandHandler::CTLCommandHandler()
        {
            this->mHandlerMap = {
                    {"EXIT", std::bind(&CTLCommandHandler::exit_handler, this, std::placeholders::_1, std::placeholders::_2)},
                    {"HELP", std::bind(&CTLCommandHandler::help_handler, this, std::placeholders::_1, std::placeholders::_2)},
                    {"SET",  std::bind(&CTLCommandHandler::set_handler,  this, std::placeholders::_1, std::placeholders::_2)},
                    {"LOG",  std::bind(&CTLCommandHandler::log_handler,  this, std::placeholders::_1, std::placeholders::_2)}
            };
        }

        //TODO think there is an easier way than this.. seems silly to rebind method for only parameter checking..
        // not sure why i did this
        std::string CTLCommandHandler::exit_handler(const std::string& cmd, const std::vector<std::string>& parameters)
        {
            if (mCommandCallbackMap.count(eCommands::EXIT) == 0)
                return "Error: No handler assigned to EXIT command!";

            mCommandCallbackMap[eCommands::EXIT]->callback_handler(cmd, parameters);
            return "Debugger connection closed...";
        }

        std::string CTLCommandHandler::help_handler(const std::string& cmd, const std::vector<std::string>& parameters)
        {
            std::string help = "Debugger commands:\n";
            for (const auto& pair : mHandlerMap) {
                help += " - " + pair.first + "\n";
            }
            return help;
        }

        std::string CTLCommandHandler::set_handler(const std::string& cmd, const std::vector<std::string>& parameters)
        {
            if (mCommandCallbackMap.count(eCommands::SET) == 0)
                return "Error: No handler assigned to SET command!";

            return mCommandCallbackMap[eCommands::SET]->callback_handler(cmd, parameters);
        }

        std::string CTLCommandHandler::log_handler(const std::string& cmd, const std::vector<std::string>& parameters)
        {
            if (mCommandCallbackMap.count(eCommands::LOG) == 0)
                return "Error: No handler assigned to LOG command!";

            return mCommandCallbackMap[eCommands::LOG]->callback_handler(cmd, parameters);
        }

    }
}