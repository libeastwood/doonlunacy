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
    enum in file in question. Use LV_AVAILABLE as basis for your enum:
    
    @code
    enum LogAiVerbosity
    {
        LV_AI_WORLDWIDE = LV_AVAILABLE,
        LV_AI_PLATOON,
        LV_AI_SQUAD
    };
    @endcode
            
*/    

#ifndef DUNE_LOG_H
#define DUNE_LOG_H

#include "singleton.h"
#include "String.h"

#include <map>
#include <boost/shared_ptr.hpp>

#ifndef LOG_DISABLED

//! Log verbosity level
/*!
    Used to filter unnecessary messages.
*/
enum LogVerbosity
{
    //! bye, bye !
    LV_FATAL = 0,
    //! that hurt...
    LV_ERROR,
    //! i can handle that
    LV_WARNING,
    //! look what's happening
    LV_INFO,
    //! first user loglevel
    LV_AVAILABLE,
    //! last loglevel (used to set full logging)
    LV_MAX = 255
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

    @see LOG, LOG_FATAL, LOG_ERROR, LOG_WARNING, LOG_INFO
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
        void logFatal(ConstString logSystem, const char *format, ...);
        void logError(ConstString logSystem, const char *format, ...);
        void logWarning(ConstString logSystem, const char *format, ...);
        void logInfo(ConstString logSystem, const char *format, ...);

        //@}


        //! @name Indentation
        //@{
        
        void indent()
        {
            indentLevel++;
        };
        void unindent()        
        {
            if (indentLevel > 0)
                indentLevel--;
        };
        
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
        int indentLevel;
        
        bool checkMessageVerbosity(ConstString logSystem, LogVerbosity verbosity);
        void doLog(ConstString logSystem, LogVerbosity verbosity, const char *format, va_list args);
        
};

// few usefull macros

//! @name Subsystem logs
//! Subsystem emitted messages
//@{

//! Log message of given verbosity emitted by given system
#define LOG           Log::Instance()->log
//! Log fatal message emitted by given system
#define LOG_FATAL     Log::Instance()->logFatal
//! Log error message emitted by given system
#define LOG_ERROR     Log::Instance()->logError
//! Log warning message emitted by given system
#define LOG_WARNING   Log::Instance()->logWarning
//! Log info message emitted by given system
#define LOG_INFO      Log::Instance()->logInfo

//! Increase indentation for following messages
#define LOG_INDENT      Log::Instance()->indent

//! Decrease indentation for following messages
#define LOG_UNINDENT    Log::Instance()->unindent

//@}

#else // LOG_DISABLED

// few useless macros
// TODO: not sure what to do with these, they raise some warnings... (otpetrik)

#define LOG           ((void)(0))
#define LOG_FATAL     ((void)(0))
#define LOG_ERROR     ((void)(0))
#define LOG_WARNING   ((void)(0))
#define LOG_INFO      ((void)(0))

#define LOG_INDENT      ((void)(0))
#define LOG_UNINDENT    ((void)(0))

#endif // LOG_DISABLED

#endif // DUNE_LOG_H
