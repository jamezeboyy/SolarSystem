

#ifndef SOLARSYSTEM_COMMAND_HANDLER_H
#define SOLARSYSTEM_COMMAND_HANDLER_H

#include <string>
#include <functional>
#include <map>
#include <vector>
#include "command_callback.h"

namespace SolarSystem
{
    namespace Core
    {

        enum eCommands
        {
            EXIT,
            HELP
        };

        class CCommandHandler
        {
        private:

            std::map<std::string, std::function<std::string(std::vector<std::string>&)>> mHandlerMap;
            std::map<eCommands, CCommandCallbacker*> mCommandCallbackMap;

        public:

            CCommandHandler();

            std::string handle(const std::string& command);

            std::string exit_handler(const std::vector<std::string>& parameters);

            std::string help_handler(const std::vector<std::string>& parameters);

            void add_callback(eCommands command, CCommandCallbacker* callbacker);


        };
    }
}


#endif //SOLARSYSTEM_COMMAND_HANDLER_H
