

#include <numeric>
#include "settercommand_handler.h"

namespace SolarSystem
{
    namespace Core
    {

        CSetterCommandHandler::CSetterCommandHandler()
        {
            this->mHandlerMap = {
                    {"OBJ1", std::bind(&CSetterCommandHandler::obj_set_handler, this, std::placeholders::_1, std::placeholders::_2)}
            };
        }

        std::string CSetterCommandHandler::callback_handler(std::string cmd, std::vector<std::string> parameters)
        {
            std::string result = std::accumulate(parameters.begin(), parameters.end(), std::string(),
                                             [](const std::string &a, const std::string &b) {
                                                     return a.empty() ? b : a + " " + b;
                                                 });
            return handle(result);
        }


        // Example
        std::string CSetterCommandHandler::obj_set_handler(const std::string cmd, const std::vector<std::string>& parameters)
        {
            if (mCommandCallbackMap.count(eCommandsSetter::OBJ1) == 0)
                return "Error: No handler assigned to OBJ1 command!";

//            return mCommandCallbackMap[eCommandsSetter::OBJ1]->callback_handler(parameters);
            return "In OBJ handler";
        }

    }
}