

#include "debug_writer.h"

#include <utility>


namespace SolarSystem
{
    namespace Core
    {

        CDebugWriter::CDebugWriter() :
                mDebugStream(std::cout),
                mErrorStream(std::cerr)
        {

        }

        CDebugWriter::CDebugWriter(std::ostream& debugStream, std::ostream& errorStream) :
                mDebugStream(debugStream),
                mErrorStream(errorStream)
        {

        }


        void CDebugWriter::log(eLogLevel logLevel, const std::string& message) const
        {
            std::ostream& stream = get_ostream_from_level(logLevel);
            stream << get_current_datetime() << " | " << message << std::endl;
        }

        void CDebugWriter::log(const std::string& message) const
        {
            log(eLogLevel::LOG_LEVEL_DEBUG, message);
        }


        std::ostream & CDebugWriter::get_ostream_from_level(eLogLevel logLevel) const
        {
            switch (logLevel)
            {
                case eLogLevel::LOG_LEVEL_DEBUG:
                    return mDebugStream;
                case eLogLevel::LOG_LEVEL_ERROR:
                    return mErrorStream;
                default:
                    return mDebugStream;
            }
        }


        std::string CDebugWriter::get_current_datetime()
        {
            auto now = std::chrono::system_clock::now();

            std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);

            auto duration = now.time_since_epoch();
            auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration) % 1000;

            std::tm now_tm{};

            localtime_s(&now_tm, &now_time_t);
            std::stringstream ss;
            ss << std::put_time(&now_tm, "%Y-%m-%d.%X")
               << '.' << std::setfill('0')
               << std::setw(3) << millis.count();
            return ss.str();
        }


    }
}