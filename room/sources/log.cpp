#include "log.h"
#include <boost/log/core.hpp>
#include <boost/log/common.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/expressions/formatters.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/support/date_time.hpp>


BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(InnerSafeLogger, CLog::SeverityLogger)

CLog::CLog()
{

}

bool CLog::init()
{
    boost::shared_ptr<CLog> initializer(new CLog());
    if(!initializer.get())
        return false;

    initializer->addConsoleLog();
    //initializer->addFileLog();
    initializer->addAttributes();

    return true;
}

CLog::SeverityLogger &CLog::logger()
{
    return InnerSafeLogger::get();
}

void CLog::addConsoleLog()
{
    typedef boost::log::sinks::synchronous_sink<boost::log::sinks::text_ostream_backend> sync_sink;
    typedef boost::shared_ptr<sync_sink> sync_sink_ptr;

    sync_sink_ptr sink = boost::log::add_console_log();
//    sink->set_formatter
//    (
//        boost::log::expressions::stream
//        << boost::log::expressions::attr<boost::log::attributes::current_thread_id::value_type>("ThreadID")
//        << " ["
//        << boost::log::expressions::format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S<%f>")
//        << "]"
//        << "<"
//        << boost::log::expressions::attr<boost::log::trivial::severity_level>("Severity")
//        << "> "
//        << boost::log::expressions::message
//    );
    sink->set_formatter
    (
        boost::log::expressions::stream
        << "["
        << boost::log::expressions::format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S.%f")
        << "] ["
        << boost::log::expressions::attr<boost::log::attributes::current_thread_id::value_type>("ThreadID")
        << "] ["
        << boost::log::expressions::attr<boost::log::trivial::severity_level>("Severity")
        << "]   "
        << boost::log::expressions::message
    );

    boost::log::core::get()->add_sink(sink);
}

void CLog::addFileLog()
{
    typedef boost::log::sinks::synchronous_sink< boost::log::sinks::text_file_backend > sync_sink;
        typedef boost::shared_ptr< sync_sink > sync_sink_ptr;

        sync_sink_ptr sink = boost::log::add_file_log
        (
            boost::log::keywords::file_name = "room_%Y%m%d.log%N",
            boost::log::keywords::rotation_size = 10 * 1024 * 1024,
            boost::log::keywords::time_based_rotation = boost::log::sinks::file::rotation_at_time_point
                        (
                            0,
                            0,
                            0
                        )
        );

        sink->locked_backend()->set_file_collector
            (
                boost::log::sinks::file::make_collector
                (
                    boost::log::keywords::target = "./logs",
                    boost::log::keywords::max_size = 500 * 1024 * 1024
                )
            );
        sink->locked_backend()->scan_for_files();

        sink->set_formatter
        (
            boost::log::expressions::stream
            << boost::log::expressions::attr<boost::log::attributes::current_thread_id::value_type>("ThreadID")
            << " ["
            << boost::log::expressions::format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S<%f>")
            << "]"
            << "<"
            << boost::log::expressions::attr<boost::log::trivial::severity_level>("Severity")
            << "> "
            << boost::log::expressions::message
        );

        sink->set_filter
        (
            boost::log::expressions::attr<boost::log::trivial::severity_level>("Severity")
            >=
            boost::log::trivial::warning
        );

        boost::log::core::get()->add_sink(sink);
}

void CLog::addAttributes()
{
    //boost::log::core::get()->add_global_attribute("ThreadID", boost::log::attributes::current_thread_id());
    //boost::log::core::get()->add_thread_attribute("Scope", boost::log::attributes::named_scope());
    //boost::log::core::get()->add_global_attribute("Scope", boost::log::attributes::named_scope());
    boost::log::add_common_attributes();
}
