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

// Boost
#include <boost/filesystem/fstream.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/from_stream.hpp>

typedef boost::log::sinks::synchronous_sink< boost::log::sinks::text_file_backend > text_sink;

te::core::Logger& te::core::Logger::getInstance()
{
  static Logger instance;

  return instance;
}

void te::core::Logger::setupFromFile(const std::string& filename)
{
  boost::log::core::get()->remove_all_sinks();
  boost::filesystem::ifstream settings(filename);
  boost::log::init_from_stream(settings);
}

void te::core::Logger::setupLogger(const std::string& filename)
{
  m_filename = filename;

  boost::log::core::get()->remove_all_sinks();

  boost::log::add_file_log(boost::log::keywords::auto_flush = true,
                           boost::log::keywords::format = "[%TimeStamp%] <%Severity%> : %Message%",
                           boost::log::keywords::file_name = m_filename + ".log",
                           boost::log:: keywords::open_mode = std::ios_base::app
                          );
  boost::log::add_common_attributes();

}

te::core::Logger::Logger()
{
  boost::log::register_simple_formatter_factory< boost::log::trivial::severity_level, char >("Severity");
  setupLogger(m_filename);
}

void te::core::Logger::log(const std::string& message, boost::log::trivial::severity_level severity)
{
  BOOST_LOG_SEV(m_logger , severity) << message;
}


