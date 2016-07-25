/*
  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/core/logger/Logger.cpp

  \brief This class is designed to manage the log of information in TerraLib.

  \author Matheus Cavassan Zaglia
  \author Gilberto Ribeiro de Queiroz
 */

// TerraLib
#include "Logger.h"
#include "../Exception.h"
#include "../translator/Translator.h"

// Boost
#include <boost/format.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/log/attributes/current_process_name.hpp>
#include <boost/log/attributes/current_thread_id.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/from_stream.hpp>

BOOST_LOG_ATTRIBUTE_KEYWORD(channel, "Channel", std::string)

te::core::Logger& te::core::Logger::instance()
{
  static Logger instance;

  return instance;
}

void te::core::Logger::addLoggerFromFile(const std::string& filename)
{
  if(filename.empty())
    te::InvalidArgumentException() << te::ErrorDescription(TE_TR("The logger configuration file name cannot be empty."));

  if(exists(filename))
  {
    boost::format err_msg(TE_TR("There is already a logger registered using the configuration in file named: %1%"));

    te::InvalidArgumentException() << te::ErrorDescription((err_msg  % filename).str());
  }

  boost::filesystem::ifstream settings(filename);
  boost::log::init_from_stream(settings);
  
  m_logger_list.push_back(filename);
}

void te::core::Logger::addLogger(const std::string& name, const std::string& filename, std::string format)
{
  if(name.empty())
    throw te::InvalidArgumentException() << te::ErrorDescription(TE_TR("The logger name cannot be empty."));

  if(exists(name))
  {
    boost::format err_msg(TE_TR("There is already a logger registered with the name: %1%"));

    throw te::InvalidArgumentException() << te::ErrorDescription((err_msg  % name).str());
  }

  if(format.empty())
    format = "[%TimeStamp%] <%Severity%>: %Message%";

  boost::log::add_file_log(boost::log::keywords::auto_flush = true,
                           boost::log::keywords::format = format,
                           boost::log::keywords::file_name = filename,
                           boost::log::keywords::channel = name,
                           boost::log::keywords::open_mode = std::ios_base::app,
                           boost::log::keywords::filter = (channel == name)
                          );

  m_logger_list.push_back(name);
}

bool te::core::Logger::exists(const std::string &name)
{
  return std::find_if(m_logger_list.begin(), m_logger_list.end(),
                      [&name](const std::string& n)
                      { return name == n; }) != m_logger_list.end();
}

void te::core::Logger::removeAllLoggers()
{
  boost::log::core::get()->remove_all_sinks();
  m_logger_list.clear();
}

te::core::Logger::Logger()
{
  boost::log::register_simple_formatter_factory< boost::log::trivial::severity_level, char >("Severity");

  boost::log::core::get()->add_global_attribute("Process",
                                                boost::log::attributes::current_process_name());
  boost::log::core::get()->add_global_attribute("ProcessID",
                                                boost::log::attributes::current_process_id());
  boost::log::core::get()->add_global_attribute("ThreadID",
                                                boost::log::attributes::current_thread_id());
  boost::log::add_common_attributes();
}

void te::core::Logger::log(const std::string& message, const std::string& channel, boost::log::trivial::severity_level severity)
{
  BOOST_LOG_CHANNEL_SEV(m_logger, channel, severity) << message;
}


