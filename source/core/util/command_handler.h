

#ifndef SOLARSYSTEM_COMMAND_HANDLER_H
#define SOLARSYSTEM_COMMAND_HANDLER_H

#include <string>
#include <functional>
#include <map>
#include <vector>
#include <deque>
#include <algorithm>
#include <sstream>
#include "command_callback.h"

namespace SolarSystem
{
    namespace Core
    {


        template<typename E> class CCommandHandler
        {
        protected:

            std::map<std::string, std::function<std::string(std::string, std::vector<std::string>&)>> mHandlerMap;
            std::map<E, CCommandCallbacker*> mCommandCallbackMap;

        public:

            CCommandHandler() = default;

            std::string handle(std::string& command)
            {
                std::string s;
                std::stringstream ss(command);

                std::deque<std::string> parsedInput;

                while (getline(ss, s, ' '))
                    parsedInput.push_back(s);

                if (parsedInput.empty()) return "Failed: Command string is empty!";

                std::string cmd = parsedInput.front();
                parsedInput.pop_front();
                std::transform(cmd.begin(), cmd.end(), cmd.begin(),
                               [](unsigned char c){ return std::toupper(c); });

                if (this->mHandlerMap.count(cmd) == 0) return "Unknown command!";

                auto handlerFunc = this->mHandlerMap[cmd];

                // TODO maybe more efficient way for this?
                std::vector<std::string> params(parsedInput.begin(), parsedInput.end());
                return handlerFunc(cmd, params);
            }


            void add_callback(E commandEnum, CCommandCallbacker* callbacker)
            {
                mCommandCallbackMap[commandEnum] = callbacker;
            }


        };

    }
}


#endif //SOLARSYSTEM_COMMAND_HANDLER_H
