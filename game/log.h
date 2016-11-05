//
// Created by serg on 11/4/16.
//

#ifndef TIC_TAC_TOE_LOG_H
#define TIC_TAC_TOE_LOG_H

#include <boost/log/trivial.hpp>

#define LOG(X) BOOST_LOG_TRIVIAL(X)

#define LOG_DEBUG BOOST_LOG_TRIVIAL(debug)
#define LOG_WARNING BOOST_LOG_TRIVIAL(warning)
#define LOG_ERROR BOOST_LOG_TRIVIAL(error)


/*#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>

namespace logging = boost::log;
namespace sinks = boost::log::sinks;
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;
namespace attrs = boost::log::attributes;
namespace keywords = boost::log::keywords;

void log_init()
{
    logging::add_file_log
            (
                    keywords::file_name = "sample_%N.log",
    keywords::rotation_size = 10 * 1024 * 1024,
    keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0),
    keywords::format = "[%TimeStamp%]: %Message%"
    );

    logging::core::get()->set_filter
            (
                    logging::trivial::severity >= logging::trivial::info
            );
}
*/
#endif //TIC_TAC_TOE_LOG_H
