

#ifndef SOLARSYSTEM_SETTERCOMMAND_HANDLER_H
#define SOLARSYSTEM_SETTERCOMMAND_HANDLER_H

#include "command_callback.h"
#include "command_handler.h"

namespace SolarSystem
{
    namespace Core
    {

        enum eCommandsSetter
        {
            OBJ1,
        };

        class CSetterCommandHandler : public CCommandCallbacker, public CCommandHandler<eCommandsSetter>
        {
        public:
            CSetterCommandHandler();

            std::string callback_handler(std::vector<std::string> parameters) override;

        private:

            std::string obj_set_handler(const std::vector<std::string>& parameters);

        };

    }
}

#endif //SOLARSYSTEM_SETTERCOMMAND_HANDLER_H
