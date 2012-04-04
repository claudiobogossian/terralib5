/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file Logger.cpp

  \brief This class is designed to manage the log of information in TerraLib.
 */

// TerraLib
#include "Logger.h"

#if TE_LOGGER_ENABLED

// TerraLib
#include "Exception.h"
#include "Translator.h"

// STL
#include <cassert>
#include <cstdlib>

// Boost
#include <boost/filesystem.hpp>

// Apache Log4CXX
#if TE_USE_APACHE_LOG4CXX
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/consoleappender.h>
#include <log4cxx/logger.h>
#include <log4cxx/logmanager.h>
#include <log4cxx/patternlayout.h>
#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/xml/domconfigurator.h>
#endif

std::string te::common::Logger::getDefaultConfigFile()
{
// let's check if there is a file called TE_LOGGER_DEFAULT_CONFIGURATION_FILE in the current application dir
  if(boost::filesystem::exists(TE_LOGGER_DEFAULT_CONFIGURATION_FILE))
    return TE_LOGGER_DEFAULT_CONFIGURATION_FILE;

// if the default file is not available in the current dir let's try an environment variable defined as TERRALIB_DIR_ENVIRONMENT_VARIABLE
  char* e = getenv(TERRALIB_DIR_ENVIRONMENT_VARIABLE);

  if(e != 0)
  {
    boost::filesystem::path p(e);
    p /= TE_LOGGER_DEFAULT_CONFIGURATION_FILE;

    if(boost::filesystem::exists(p))
      return p.string();
  }

// Humm: default config file not found
  return "";
}

void te::common::Logger::initialize(const std::string& loggerName,
                                    const LoggerConfigurationType t,
                                    const std::string& fileName)
{
#if TE_USE_APACHE_LOG4CXX
  finalize(loggerName);

  if(fileName.empty())
    return;

  //if(fileName.empty())
  //  throw Exception(TR_COMMON("You must specify a logger configuration file!"));

  log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger(loggerName));

  if(logger == 0)
    throw Exception(TR_COMMON("It was not possible to initialize the logger!"));

  if(t == LOGGER_XML_CONFIG)
  {
    log4cxx::xml::DOMConfigurator::configure(fileName);
  }
  else if(t == LOGGER_TXT_CONFIG)
  {
    log4cxx::PropertyConfigurator::configure(fileName);
  }
  else
  {
    throw Exception(TR_COMMON("Invalid logger configuration type!"));
  }
#endif
}

void te::common::Logger::initialize(const std::string& loggerName)
{
#if TE_USE_APACHE_LOG4CXX
  finalize(loggerName);

  log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger(loggerName));

  if(logger == 0)
    throw Exception(TR_COMMON("It was not possible to initialize the the logger!"));
#endif
}

void te::common::Logger::finalize(const std::string& loggerName)
{
#if TE_USE_APACHE_LOG4CXX
  if(loggerName.empty())
    throw Exception(TR_COMMON("The logger name is empty!"));

  log4cxx::LoggerPtr logger = log4cxx::LogManager::exists(loggerName);

  if(logger == 0)
    return; // the logger doesn't exist in the hierarchy, so we don't need to finalize anything!

  logger->removeAllAppenders();
#endif
}

void te::common::Logger::logFatal(const char* logger, const char* msg)
{
#if TE_USE_APACHE_LOG4CXX
  log4cxx::LoggerPtr lo(log4cxx::Logger::getLogger(logger));
  LOG4CXX_FATAL(lo, msg); 
#endif
}

void te::common::Logger::logFatal(const std::string& logger, const std::string& msg)
{
  logFatal(logger.c_str(), msg.c_str());
}

void te::common::Logger::logAssert(const char* logger, bool condition, const char* msg)
{
#if TE_USE_APACHE_LOG4CXX
  log4cxx::LoggerPtr lo(log4cxx::Logger::getLogger(logger));
  LOG4CXX_ASSERT(lo, condition, msg);
#endif
}

void te::common::Logger::logError(const char* logger, const char* msg)
{
#if TE_USE_APACHE_LOG4CXX
  log4cxx::LoggerPtr lo(log4cxx::Logger::getLogger(logger));
  LOG4CXX_ERROR(lo, msg);
#endif
}

void te::common::Logger::logWarning(const char* logger, const char* msg)
{
#if TE_USE_APACHE_LOG4CXX
  log4cxx::LoggerPtr lo(log4cxx::Logger::getLogger(logger));
  LOG4CXX_WARN(lo, msg);
#endif
}

void te::common::Logger::logInfo(const char* logger, const char* msg)
{
#if TE_USE_APACHE_LOG4CXX
  log4cxx::LoggerPtr lo(log4cxx::Logger::getLogger(logger));
  LOG4CXX_INFO(lo, msg);
#endif
}

void te::common::Logger::logInfo(const std::string& logger, const std::string& msg)
{
  logInfo(logger.c_str(), msg.c_str());
}

void te::common::Logger::logDebug(const char* logger, const char* msg)
{
#if TE_USE_APACHE_LOG4CXX
  log4cxx::LoggerPtr lo(log4cxx::Logger::getLogger(logger));
  LOG4CXX_DEBUG(lo, msg);
#endif
}

void te::common::Logger::logTrace(const char* logger, const char* msg)
{
#if TE_USE_APACHE_LOG4CXX
  log4cxx::LoggerPtr lo(log4cxx::Logger::getLogger(logger));
  LOG4CXX_TRACE(lo, msg);
#endif
}

void te::common::Logger::logTrace(const std::string& logger, const std::string& msg)
{
  logTrace(logger.c_str(), msg.c_str());
}

#endif  // TE_LOGGER_ENABLED

