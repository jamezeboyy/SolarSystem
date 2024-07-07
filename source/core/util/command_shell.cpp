#include <windows.h>
#include <iostream>
#include <string>
#include <algorithm>

const char* COMMAND_PIPE_NAME = R"(\\.\pipe\SolarSystemDebugCommandPipe)";
const char* RESPONSE_PIPE_NAME = R"(\\.\pipe\SolarSystemDebugResponsePipe)";
const char* NAME = R"( _____   _____   _       _____   _____
|  ___| |  _  | | |     |  _  | |  _  |
| |___  | | | | | |     | |_| | | |_| |
|___  | | | | | | |     |  _  | |    _|
 ___| | | |_| | | |___  | | | | | |\ \
|_____| |_____| |_____| |_| |_| |_| |_|
 _____   _____   _____   _   _   ______    ______   _____   _____
|  _  \ | ____| |  _  \ | | | | |  ____|  |  ____| | ____| |  _  |
| | | | | |___  | |_| / | | | | | |  ___  | |  ___ | |___  | |_| |
| | | | |  ___| |  _  \ | | | | | | |_ _| | | |_ _||  ___| |    _|
| |_| | | |___  | |_| | | |_| | | |__| |  | |__| | | |___  | |\ \
|_____/ |_____| |_____/ |_____| |______|  |______| |_____| |_| |_|

=======================================================================)";


void sendCommand(HANDLE hPipe, HANDLE hResponsePipe)
{
    std::string command;
    while (true)
    {
        try
        {
            std::cout << "> ";

            std::getline(std::cin, command);
            if (command.empty()) continue;

            DWORD bytesWritten;
            bool result = WriteFile(hPipe, command.c_str(), command.length(), &bytesWritten, nullptr);
            if (!result)
            {
                throw std::runtime_error("Failed to write to pipeline.");
            }
            std::string cmd = command;
            std::transform(cmd.begin(), cmd.end(), cmd.begin(),
                           [](unsigned char c){ return std::toupper(c); });

            if (cmd == "EXIT")
            {
                break;
            }

            char buffer[1024];
            DWORD bytesRead;
            result = ReadFile(hResponsePipe, buffer, sizeof(buffer) - 1, &bytesRead, nullptr);
            if (result)
            {
                if (bytesRead > 0)
                {
                    buffer[bytesRead] = '\0';
                    std::cout << buffer << std::endl;
                }
                else
                {
                    throw std::runtime_error("Received empty response from SolarSystem.");
                }
            }
            else
            {
                DWORD error = GetLastError();
                if (error == ERROR_BROKEN_PIPE)
                {
                    throw std::runtime_error("Pipe connection closed by SolarSystem.");
                }
                else
                {
                    throw std::runtime_error("Error reading response from SolarSystem. " + std::to_string(error));
                }
            }
        }
        catch (const std::exception& e)
        {
            std::cerr << "Exception caught: " << e.what() << "\n";
        }
    }
}

int main() {
    std::cout << NAME << std::endl;

    HANDLE hCommandPipe = CreateFile(
            COMMAND_PIPE_NAME,
            GENERIC_WRITE,
            0,
            nullptr,
            OPEN_EXISTING,
            0,
            nullptr
    );

    HANDLE hResponsePipe = CreateFile(
            RESPONSE_PIPE_NAME,
            GENERIC_READ,
            0,
            nullptr,
            OPEN_EXISTING,
            0,
            nullptr
    );

    if (hResponsePipe == INVALID_HANDLE_VALUE || hCommandPipe == INVALID_HANDLE_VALUE)
    {
        std::cerr << "Failed to connect to SolarSystem, check that the connection is open (" << GetLastError() << ").\n";
        CloseHandle(hCommandPipe);
        CloseHandle(hResponsePipe);
        return -1;
    }

    std::cout << "Connected to the SolarSystem process. Type commands below:\n";

    sendCommand(hCommandPipe, hResponsePipe);

    CloseHandle(hCommandPipe);
    CloseHandle(hResponsePipe);
    return 0;
}
