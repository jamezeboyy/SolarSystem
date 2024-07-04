#include <windows.h>
#include <iostream>
#include <thread>
#include <string>

#include "core/util/console_handler.h"


int main(int argc, char* argv[])
{
    bool startConsole = false;
    if (argc == 2)
        if(strcmp(argv[1], "debug") == 0)
            startConsole = true;


    using namespace SolarSystem::Core;
    CConsoleHandler consoleHandler = CConsoleHandler();

    consoleHandler.start(startConsole);

    consoleHandler.stop();
}

