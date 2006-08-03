/*!
    \file Log.h
    \brief Message logging routines.   

    @note
    Avoid use of global logging, try using subsystem specific. It allows finer
    verbosity control.

    @note
    To disable logging altogether, define macro LOG_DISABLED.

    @note
    Do not add new log verbosity levels to this file, instead create your own 
    enum in file in question. Use LOG_LEVEL_AVAILABLE as basis for your enum:
    
    @code
    enum LogAiVerbosity
    {
        LOG_AI_WORLDWIDE = LOG_VERBOSITY_AVAILABLE,
        LOG_AI_PLATOON,
        LOG_AI_SQUAD
    };
    @endcode
    
    @note
    To make logging easier for subsystem SOMETHING, define your own macros:
    
    @code

    #define SOMETHING_LOG(level,...)   S_LOG("something", level, __VA_ARGS__)
    #define SOMETHING_LOG_ERROR(...)   SOMETHING_LOG(LOG_ERROR, __VA_ARGS__)
    #define SOMETHING_LOG_FATAL(...)   SOMETHING_LOG(LOG_FATAL, __VA_ARGS__)
    #define SOMETHING_LOG_WARNING(...) SOMETHING_LOG(LOG_WARNING, __VA_ARGS__)
    #define SOMETHING_LOG_INFO(...)    SOMETHING_LOG(LOG_INFO, __VA_ARGS__)
    
    @endcode
        
*/    

#ifndef DUNE_LOG_H
#define DUNE_LOG_H

#include "singleton.h"

#include <string>
#include <map>
#include <boost/shared_ptr.hpp>

// TODO: not sure where these belong, mayby separate file Types.h ? (otpetrik)
typedef std::string String;
typedef const std::string &ConstString;

#ifndef LOG_DISABLED

//! Log verbosity level
/*!
    Used to filter unnecessary messages.
*/
enum LogVerbosity
{
    //! bye, bye !
    LOG_FATAL = 0,
    //! that hurt...
    LOG_ERROR,
    //! i can handle that
    LOG_WARNING,
    //! look what's happening
    LOG_INFO,
    //! first user loglevel
    LOG_VERBOSITY_AVAILABLE,
    //! last loglevel (used to set full logging)
    LOG_VERBOSITY_MAX = 255
};

class LogBackend;

typedef boost::shared_ptr<LogBackend> LogBackendPtr;

//! Generic log backend class
/*!
    Parent class for all log backends (write to console,
    file, screen, network, ...)
*/
class LogBackend
{
    public:
        //! @name Constructors & Destructor
        //@{
    
        //! Constructor
        LogBackend();
        //! Destructor
        virtual ~LogBackend();

        //@}
        
        //! @name Log methods
        //@{

        //! Log the message
        /*!
            Called by Log class to log every feasible message.
            
            @param message message to be logged
        */
        virtual void log(const char *message) = 0;
        
        //@}
};

//! Stdout backend class
/*!
    Default backend, just writes message to the console.
*/
class LogBackendStdout : public LogBackend
{
    public:
        virtual void log(const char *message);
};

//! Log system
/*!
    Provides complete logging facilites including different verbosity levels.
    
    @note: For logging itself, do not use this class, use macros !

    @see S_LOG, S_LOG_FATAL, S_LOG_ERROR, S_LOG_WARNING, S_LOG_INFO
    @see G_LOG, G_LOG_FATAL, G_LOG_ERROR, G_LOG_WARNING, G_LOG_INFO
*/
class Log : public Singleton<Log>
{
    friend class Singleton<Log>;   
    
    public:

        //! @name Logging
        //@{

        //! Log message
        /*!
            @param logSystem string identificatin what logged the message
            @param verbosity verbosity of message
            @param message message itself 
        */
        void log(ConstString logSystem, LogVerbosity verbosity, ConstString message);
        //! Log message
        /*!
            @param logSystem string identificatin what logged the message
            @param verbosity verbosity of message
            @param format printf-type message string             
        */
        void log(ConstString logSystem, LogVerbosity verbosity, const char *format, ...);

        //@}


        //! @name Verbosity setting
        //@{
        
        //! Set verbosity level for given system
        void setVerbosity(ConstString logSystem, LogVerbosity verbosity);
        
        //! Set default verbosity level
        /*
            Sets verbosity level for system with no verbosity level on their own
        */
        void setDefaultVerbosity(LogVerbosity verbosity) { defaultVerbosity = verbosity; };

        //@}

        //! @name Backend
        //@{
        
        //! Set new backend
        void setBackend(LogBackendPtr backend);        
        //! Get current backend
        LogBackendPtr getBackend() { return backend; };

        //@}

    protected:
    
        //! @name Constructors & Destructor
        //@{
    
        //! Constructor
        Log();
        //! Destructor
        ~Log();

        //@}
        
    private:
        LogVerbosity defaultVerbosity;
        LogBackendPtr backend;
        std::map<const String, LogVerbosity> verbosities;
        
        void doLog(ConstString logSystem, LogVerbosity verbosity, const char *message);
        
};

// few usefull macros

//! @name Subsystem logs
//! Subsystem emitted messages
//@{

//! Log message of given verbosity emitted by given system
#define S_LOG(system,verbosity,...) Log::Instance()->log(system, verbosity, __VA_ARGS__)
//! Log fatal message emitted by given system
#define S_LOG_FATAL(system,...) Log::Instance()->log(system, LOG_FATAL, __VA_ARGS__)
//! Log error message emitted by given system
#define S_LOG_ERROR(system,...) Log::Instance()->log(system, LOG_ERROR, __VA_ARGS__)
//! Log warning message emitted by given system
#define S_LOG_WARNING(system,...) Log::Instance()->log(system, LOG_WARNING, __VA_ARGS__)
//! Log info message emitted by given system
#define S_LOG_INFO(system,...) Log::Instance()->log(system, LOG_INFO, __VA_ARGS__)

//@}

//! @name Global log
//! Messages not bound to any given system
//@{

//! Log message of given verbosity
#define G_LOG(level,...) S_LOG("", level, __VA_ARGS__)
//! Log fatal message
#define G_LOG_FATAL(...) G_LOG(LOG_FATAL, __VA_ARGS__)
//! Log error message
#define G_LOG_ERROR(...) G_LOG(LOG_ERROR, __VA_ARGS__)
//! Log warning message
#define G_LOG_WARNING(...) G_LOG(LOG_WARNING, __VA_ARGS__)
//! Log info message
#define G_LOG_INFO(...) G_LOG(LOG_INFO, __VA_ARGS__)

//@}

#else // LOG_DISABLED

// few useless macros

#define S_LOG(system,verbosity,...) ((void)(0))
#define S_LOG_FATAL(system,...)     ((void)(0))
#define S_LOG_ERROR(system,...)     ((void)(0))
#define S_LOG_WARNING(system,...)   ((void)(0))
#define S_LOG_INFO(system,...)      ((void)(0))
#define G_LOG(level,...)            ((void)(0))
#define G_LOG_FATAL(...)            ((void)(0))
#define G_LOG_ERROR(...)            ((void)(0))
#define G_LOG_WARNING(...)          ((void)(0))
#define G_LOG_INFO(...)             ((void)(0))


#endif // LOG_DISABLED

#endif // DUNE_LOG_H
