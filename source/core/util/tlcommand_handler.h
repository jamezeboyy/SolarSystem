

#ifndef SOLARSYSTEM_TLCOMMAND_HANDLER_H
#define SOLARSYSTEM_TLCOMMAND_HANDLER_H

#include "command_handler.h"

namespace SolarSystem
{
    namespace Core
    {
        enum eCommands
        {
            EXIT,
            HELP,
            SET
        };


        class CTLCommandHandler : public CCommandHandler<eCommands>
        {
        public:
            CTLCommandHandler();

        private:
            std::string exit_handler(const std::vector<std::string>& parameters);

            std::string help_handler(const std::vector<std::string>& parameters);

            std::string set_handler(const std::vector<std::string>& parameters);

        };

    }
}

#endif //SOLARSYSTEM_TLCOMMAND_HANDLER_H
