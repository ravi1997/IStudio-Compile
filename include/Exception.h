#pragma once

#include "Types_Compiler.hpp"
#include "backward.hpp"
using namespace backward;



namespace IStudio::Exception
{

    class Exception : public std::exception
    {
    public:
        Exception(const std::string &message,
                            const std::string &name = "Exception",
                            const std::source_location &sourceLocation =
                                std::source_location::current())
            : m_name(name), m_message(message), m_sourceLocation(sourceLocation)
        {
            m_stackTrace.load_here(32);
        }

        Exception(const std::source_location &sl =
                      std::source_location::current()) noexcept
            : m_name("Exception"),
              m_message("Unknown exception"),
              m_sourceLocation(sl)
        {
            m_stackTrace.load_here(32);
        }

        const char *what() const noexcept override
        {
            return m_message.c_str();
        }

        friend std::ostream &operator<<(std::ostream &os,
                                                  const Exception &ex)
        {
            os << ex.m_name << ": " << ex.m_message << std::endl;
            os << "    at " << ex.m_sourceLocation.file_name() << ":"
               << ex.m_sourceLocation.line() << std::endl;
            TraceResolver tr;
            tr.load_stacktrace(ex.m_stackTrace);
            for (size_t i = 0; i < ex.m_stackTrace.size(); ++i)
            {
                ResolvedTrace trace = tr.resolve(ex.m_stackTrace[i]);
                os << "#" << i
                   << " " << trace.object_filename
                   << " " << trace.object_function
                   << " [" << trace.addr << "]"
                   << std::endl;
            }
            return os;
        }

    private:
        std::string m_name;
        std::string m_message;
        std::source_location m_sourceLocation;
        StackTrace m_stackTrace;
    };
}
