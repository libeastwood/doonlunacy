#include "Log.h"

#ifndef LOG_DISABLED

#include <stdio.h>
#include <stdarg.h>

#define LOG_DEFAULT_VERBOSITY LV_MAX
#define LOG_MAX_STRING_LENGTH 1000

//------------------------------------------------------------------------------
// LogBackend class
//------------------------------------------------------------------------------

LogBackend::LogBackend()
{
}
LogBackend::~LogBackend()
{
}
        
void LogBackend::log(const char *message)
{
}

//------------------------------------------------------------------------------
// LogBackendStdout class
//------------------------------------------------------------------------------

void LogBackendStdout::log(const char *message)
{
    printf("%s", message);
    fflush(stdout);
}

//------------------------------------------------------------------------------
// Log class
//------------------------------------------------------------------------------
   
void Log::log(ConstString logSystem, LogVerbosity verbosity, ConstString message)
{
    doLog(logSystem, verbosity, "%s", (char *)message.c_str());
}
void Log::log(ConstString logSystem, LogVerbosity verbosity, const char *format, ...)
{
    va_list args;
    va_start(args, format);

    // try to log it
    if (checkMessageVerbosity(logSystem, verbosity))
        doLog(logSystem, verbosity, format, args);
        
    va_end(args);
}
void Log::logFatal(ConstString logSystem, const char *format, ...)
{
    va_list args;
    va_start(args, format);

    // try to log it
    if (checkMessageVerbosity(logSystem, LV_FATAL))
        doLog(logSystem, LV_FATAL, format, args);
        
    va_end(args);
}
void Log::logError(ConstString logSystem, const char *format, ...)
{
    va_list args;
    va_start(args, format);

    // try to log it
    if (checkMessageVerbosity(logSystem, LV_ERROR))
        doLog(logSystem, LV_ERROR, format, args);
        
    va_end(args);
}
void Log::logWarning(ConstString logSystem, const char *format, ...)
{
    va_list args;
    va_start(args, format);

    // try to log it
    if (checkMessageVerbosity(logSystem, LV_WARNING))
        doLog(logSystem, LV_WARNING, format, args);
        
    va_end(args);
}
void Log::logInfo(ConstString logSystem, const char *format, ...)
{
    va_list args;
    va_start(args, format);

    // try to log it
    if (checkMessageVerbosity(logSystem, LV_INFO))
        doLog(logSystem, LV_INFO, format, args);
        
    va_end(args);
}
       
void Log::setVerbosity(ConstString logSystem, LogVerbosity verbosity)
{
    verbosities[logSystem] = verbosity;
}        
        
void Log::setBackend(LogBackendPtr backend)
{
    assert(backend != NULL);
    this->backend = backend;    
}

Log::Log() : defaultVerbosity(LOG_DEFAULT_VERBOSITY), backend(new LogBackendStdout()), indentLevel(0)
{
}
Log::~Log()
{
}

bool Log::checkMessageVerbosity(ConstString logSystem, LogVerbosity verbosity)
{
    std::map<const String, LogVerbosity>::iterator i;
    LogVerbosity systemVerbosity;

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
        return false;
        
    return true;
}

void Log::doLog(ConstString logSystem, LogVerbosity verbosity, const char *format, va_list args)
{

    static char message[LOG_MAX_STRING_LENGTH];
    static char formated[LOG_MAX_STRING_LENGTH];
    const char *verb;
    
    switch (verbosity)
    {
        case LV_FATAL:
            verb = "[FATAL]";
            break;
        case LV_ERROR:
            verb = "[ERROR]";
            break;
        case LV_WARNING:
            verb = "[WARNING]";
            break;
        case LV_INFO:
        default:
            verb = "";
            break;
    };
    
    for (int i = 0; i < indentLevel; i++)
        message[i] = ' ';

    // Windows are inherently insecure (no vsnprintf & snprintf)
    // TODO: if you are using mingw, tell us whether you have snprintf & vsnprint available, thanks !
    #if defined(_WIN32) || defined(WIN32) || defined(_MSC_VER)

    vsprintf(&message[indentLevel], format, args);

    // do not print ':' unless there is a logSystem string
    if (logSystem.size() != 0)
        sprintf(formated, "%s%s: %s\n", verb, logSystem.c_str(), message);
    else
        sprintf(formated, "%s%s\n", verb, message);    

    #else
    
    vsnprintf(&message[indentLevel], LOG_MAX_STRING_LENGTH - indentLevel, format, args);

    // do not print ':' unless there is a logSystem string
    if (logSystem.size() != 0)
        snprintf(formated, LOG_MAX_STRING_LENGTH, "%s%s: %s\n", verb, logSystem.c_str(), message);
    else
        snprintf(formated, LOG_MAX_STRING_LENGTH, "%s%s\n", verb, message);    
    
    #endif
     
    backend->log(formated);
}

#endif
