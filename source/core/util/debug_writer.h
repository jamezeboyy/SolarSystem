

#ifndef SOLARSYSTEM_DEBUG_WRITER_H
#define SOLARSYSTEM_DEBUG_WRITER_H


#include <string>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace SolarSystem
{
    namespace Core
    {

        #define LOG_DECLARE CDebugWriter mDebugWriter;
        #define LOG_INIT(debugStream, errorStream) mDebugWriter(debugStream, errorStream);

        #define LOG(message) mDebugWriter.log((std::stringstream{} << message).str());
        #define LOGL(LEVEL, message) mDebugWriter.log(LEVEL, (std::stringstream{} << message).str());

        #define LOG_POINT() mDebugWriter.log((std::stringstream{} << __FILE__ << ":" << __LINE__));

        enum eLogLevel
        {
            LOG_LEVEL_DEBUG,
            LOG_LEVEL_ERROR
        };


        class CDebugWriter
        {
        private:
            std::ostream& mDebugStream;
            std::ostream& mErrorStream;

        public:

            CDebugWriter();

            CDebugWriter(std::ostream& debugStream, std::ostream& errorStream);

            void log(eLogLevel logLevel, const std::string& message) const;

            void log(const std::string& message) const;

        private:

            std::ostream & get_ostream_from_level(eLogLevel logLevel) const;

            static std::string get_current_datetime();

        };

    }
}


#endif //SOLARSYSTEM_DEBUG_WRITER_H
