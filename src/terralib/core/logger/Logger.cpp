/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled applications.

    TerraLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with TerraLib. See COPYING. If not, write to
    TerraLib Team at <terralib-team@terralib.org>.
 */

/*!
  \file terralib/core/translator/Translator.cpp

  \brief This class is designed for dealing with multi-language text translation in TerraLib.

  \author Matheus Cavassan Zaglia
  \author Gilberto Ribeiro de Queiroz
 */


// TerraLib
#include "Logger.h"

// Boost
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions/formatters/date_time.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sources/channel_feature.hpp>
#include <boost/log/sources/channel_logger.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/make_shared.hpp>

BOOST_LOG_ATTRIBUTE_KEYWORD(timestamp, "TimeStamp", boost::posix_time::ptime)
BOOST_LOG_ATTRIBUTE_KEYWORD(channel, "Channel", std::string)
BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", boost::log::trivial::severity_level)

te::core::Logger::Logger(const std::string& name)
{
    m_logger = boost::log::sources::severity_channel_logger_mt
               <boost::log::trivial::severity_level, std::string>(boost::log::keywords::channel = name);

    boost::log::formatter formatter =
                             boost::log::expressions::stream
                          << boost::log::expressions::format_date_time(timestamp, "[%Y-%m-%d %H:%M:%S]") << " <"
                          << boost::log::trivial::severity << "> : "
                          << boost::log::expressions::smessage;

    boost::log::add_file_log(boost::log::keywords::auto_flush = true,
                             boost::log::keywords::file_name = name+"%2N.log",
                             boost::log::keywords::target = "logs",
                             boost::log::keywords::max_size = 10 * 1024 * 1024,
                             boost::log::keywords::rotation_size = 10 * 1024 * 1024,
                             boost::log::keywords::scan_method = boost::log::sinks::file::scan_matching,
                             boost::log::keywords::open_mode = std::ios_base::app,
                             boost::log::keywords::filter = channel == name
                             )->set_formatter(formatter);
    boost::log::add_common_attributes();
}

te::core::Logger::~Logger()
{

}

void te::core::Logger::logTrace(const std::string &message)
{
  BOOST_LOG_SEV(m_logger, boost::log::trivial::trace) << message;
}

void te::core::Logger::logDebug(const std::string &message)
{
  BOOST_LOG_SEV(m_logger, boost::log::trivial::debug) << message;
}

void te::core::Logger::logError(const std::string &message)
{
  BOOST_LOG_SEV(m_logger, boost::log::trivial::error) << message;
}

void te::core::Logger::logInfo(const std::string &message)
{
  BOOST_LOG_SEV(m_logger, boost::log::trivial::info) << message;
}

void te::core::Logger::logFatal(const std::string &message)
{
  BOOST_LOG_SEV(m_logger, boost::log::trivial::fatal) << message;
}

void te::core::Logger::logWarning(const std::string &message)
{
  BOOST_LOG_SEV(m_logger, boost::log::trivial::warning) << message;
}
