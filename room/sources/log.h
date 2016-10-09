#ifndef CLOG_H
#define CLOG_H
#include "common.h"
#include <boost/log/trivial.hpp>
#include <boost/log/sources/logger.hpp>

#define LOG_TRACE() \
    BOOST_LOG_SEV(CLog::logger(), boost::log::trivial::trace) \
    << __FUNCTION__ \
    << "(" \
    << __FILE__ \
    << ":" \
    << __LINE__ \
    << "): "

#define LOG_DEBUG() \
    BOOST_LOG_SEV(CLog::logger(), boost::log::trivial::debug) \
    << __FUNCTION__ \
    << "(" \
    << __FILE__ \
    << ":" \
    << __LINE__ \
    << "): "

#define LOG_INFO() \
    BOOST_LOG_SEV(CLog::logger(), boost::log::trivial::info) \
    << __FUNCTION__ \
    << "(" \
    << __FILE__ \
    << ":" \
    << __LINE__ \
    << "): "

#define LOG_WARNING() \
    BOOST_LOG_SEV(CLog::logger(), boost::log::trivial::warning) \
    << __FUNCTION__ \
    << "(" \
    << __FILE__ \
    << ":" \
    << __LINE__ \
    << "): "

#define LOG_ERROR() \
    BOOST_LOG_SEV(CLog::logger(), boost::log::trivial::error) \
    << __FUNCTION__ \
    << "(" \
    << __FILE__ \
    << ":" \
    << __LINE__ \
    << "): "

#define LOG_FATAL() \
    BOOST_LOG_SEV(CLog::logger(), boost::log::trivial::fatal) \
    << __FUNCTION__ \
    << "(" \
    << __FILE__ \
    << ":" \
    << __LINE__ \
    << "): "



class CLog
{
    CLog();
public:
    typedef boost::log::sources::severity_logger_mt<boost::log::trivial::severity_level> SeverityLogger;

    static bool init();
    static SeverityLogger &logger();

    void addConsoleLog();
    void addFileLog();
    void addAttributes();

public:
    class AutoLog
    {
    public:
        AutoLog(const std::string &function, const std::string &file, long line) :
            _function(function),
            _file(file),
            _line(line)
        {
            BOOST_LOG_SEV(CLog::logger(), boost::log::trivial::trace)
                    << _function
                    << "("
                    << _file
                    << ":"
                    << _line
                    << "): Enter";
        }

        ~AutoLog()
        {
            BOOST_LOG_SEV(CLog::logger(), boost::log::trivial::trace)
                    << _function
                    << "("
                    << _file
                    << ":"
                    << _line
                    << "): Leave";
        }

    private:
        std::string _function;
        std::string _file;
        long _line;
    };
};

#endif // ASLOG_H
