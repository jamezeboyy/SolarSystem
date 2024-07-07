

#ifndef SOLARSYSTEM_COMMAND_CALLBACK_H
#define SOLARSYSTEM_COMMAND_CALLBACK_H

#include <string>
#include <vector>

namespace SolarSystem
{
    namespace Core
    {
        class CCommandCallbacker
        {
        public:
            virtual std::string callback_handler(std::string cmd, std::vector<std::string> parameters) = 0;

        };
    }
}



#endif //SOLARSYSTEM_COMMAND_CALLBACK_H
