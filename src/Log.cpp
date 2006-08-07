#include "Log.h"

#ifndef LOG_DISABLED

#include
<stdio.h>
#include <stdarg.h>

#define LOG_DEFAULT_VERBOSITY LOG_VERBOSITY_MAX
#define LOG_MAX_STRING_LENGTH 100

//------------------------------------------------------------------------------
// LogBackend class
//------------------------------------------------------------------------------

LogBackend::LogBackend()
{
};
LogBackend::~LogBackend()
{
};
        
void LogBackend::log(const char *message)
{
};

//------------------------------------------------------------------------------
// LogBackendStdout class
//------------------------------------------------------------------------------

void LogBackendStdout::log(const char *message)
{
    printf("%s", message);
};

//------------------------------------------------------------------------------
// Log class
//------------------------------------------------------------------------------
   
void Log::log(ConstString logSystem, LogVerbosity verbosity, ConstString message)
{
    doLog(logSystem, verbosity, message.c_str());
};
void Log::log(ConstString logSystem, LogVerbosity verbosity, const char *format, ...)
{
    std::map<const String, LogVerbosity>::iterator i;
    LogVerbosity systemVerbosity;
    static char message[LOG_MAX_STRING_LENGTH];

    // find verbosity level applicable to this message   
    i = verbosities.find(logSystem);
    if (i != verbosities.end())
    {
        systemVerbosity = i->second;
    }
    else
    {
        systemVerbosity = defaultVerbosity;
    }

    // check that level
    if (verbosity > systemVerbosity)
        return;


    // assemble the message
    va_list args;
    va_start(args, format);
    vsnprintf(message, LOG_MAX_STRING_LENGTH, format, args);
    va_end(args);

    // log it
    doLog(logSystem, verbosity, message);    
};
        
void Log::setVerbosity(ConstString logSystem, LogVerbosity verbosity)
{
    verbosities[logSystem] = verbosity;
};        
        
void Log::setBackend(LogBackendPtr backend)
{
    assert(backend != NULL);
    this->backend = backend;    
};

Log::Log() : defaultVerbosity(LOG_DEFAULT_VERBOSITY), backend(new LogBackendStdout())
{
};
Log::~Log()
{
};

void Log::doLog(ConstString logSystem, LogVerbosity verbosity, const char *message)
{
    static char formated[LOG_MAX_STRING_LENGTH];
    const char *verb;
    
    switch (verbosity)
    {
        case LOG_FATAL:
            verb = "[FATAL]";
            break;
        case LOG_ERROR:
            verb = "[ERROR]";
            break;
        case LOG_WARNING:
            verb = "[WARNING]";
            break;
        case LOG_INFO:
        default:
            verb = "";
            break;
    };

    // do not print ':' unless there is a logSystem string
    if (logSystem.size() != 0)
        snprintf(formated, LOG_MAX_STRING_LENGTH, "%s%s: %s\n", verb, logSystem.c_str(), message);
    else
        snprintf(formated, LOG_MAX_STRING_LENGTH, "%s%s\n", verb, message);    
     
    backend->log(formated);
}

#endif
