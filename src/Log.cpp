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
   /*
void Log::log(ConstString logSystem, LogVerbosity verbosity, ConstString message)
{
    doLog(logSystem, verbosity, "%s", (char *)message.c_str());
}*/
void Log::log(LogVerbosity verbosity, ConstString logSystem, const char *format, ...)
{
    va_list args;
    va_start(args, format);

    // try to log it
    if (checkMessageVerbosity(logSystem, verbosity))
        doLog(logSystem, verbosity, format, args);
        
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


void vsPrintf(char *str, const char *format, va_list args) {

    String output;
    for (; *format != '\0'; format++)
    {
	char curChar = *format;
	if(curChar == '%')
	{
	    char fmt[3] = {curChar, *(++format), '\0'};
	    if(fmt[1] == 'S')
	    {
		output += *va_arg(args, String*);
	    } else {
    		char buf[LOG_MAX_STRING_LENGTH];
    		void *tmp = va_arg(args, void*);
	    	// Windows is inherently insecure (no vsnprintf & snprintf)
		// TODO: if you are using mingw, tell us whether you have snprintf & vsnprint available, thanks !
#if defined(_WIN32) || defined(WIN32) || defined(_MSC_VER)
		sprintf(buf, fmt, tmp);
#else
    		snprintf(buf, LOG_MAX_STRING_LENGTH, fmt, tmp);
#endif
    		output += buf;
	    }
	    continue;
	}
	output += curChar;
    }
    va_end(args);
#if defined(_WIN32) || defined(WIN32) || defined(_MSC_VER)
    sprintf(str, "%s", output.c_str());
#else
    snprintf(str, LOG_MAX_STRING_LENGTH, "%s", output.c_str());
#endif

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
	case LV_DEBUG:
        default:
            verb = "";
            break;
    };
    
    for (int i = 0; i < indentLevel; i++)
        message[i] = ' ';

    vsPrintf(&message[indentLevel], format, args);

    // Windows are inherently insecure (no vsnprintf & snprintf)
    // TODO: if you are using mingw, tell us whether you have snprintf & vsnprint available, thanks !
    #if defined(_WIN32) || defined(WIN32) || defined(_MSC_VER)

    // do not print ':' unless there is a logSystem string
    if (logSystem.size() != 0)
        sprintf(formated, "%s%s: %s\n", verb, logSystem.c_str(), message);
    else
        sprintf(formated, "%s%s\n", verb, message);    

    #else

    // do not print ':' unless there is a logSystem string
    if (logSystem.size() != 0)
        snprintf(formated, LOG_MAX_STRING_LENGTH, "%s%s: %s\n", verb, logSystem.c_str(), message);
    else
        snprintf(formated, LOG_MAX_STRING_LENGTH, "%s%s\n", verb, message);    
    
    #endif
     
    backend->log(formated);
}

#endif
