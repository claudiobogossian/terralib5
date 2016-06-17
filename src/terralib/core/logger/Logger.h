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
  \file terralib/core/logger/Logger.h

  \brief This class is designed to manage the log of information in TerraLib.

  \author Matheus Cavassan Zaglia
  \author Gilberto Ribeiro de Queiroz
*/

#ifndef __TERRALIB_LOGGER_LOGGER_H__
#define __TERRALIB_LOGGER_LOGGER_H__

// Boost
#include <boost/log/trivial.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/sources/severity_channel_logger.hpp>
#include <boost/log/sources/channel_feature.hpp>
#include <boost/log/sources/channel_logger.hpp>
#include <boost/log/utility/setup/file.hpp>

// TerraLib
#include "../../BuildConfig.h"
#include "Config.h"

/*!
  \def TERRALIB_DEFAULT_LOGGER

  \brief The default name of the log file if none is informed.
  */
#define TERRALIB_DEFAULT_LOGGER "logs/terralib.log"

/*!
  \def TERRALIB_DEFAULT_LOGGER_FORMAT

  \brief The default message format if none is informed.
  */
#define TERRALIB_DEFAULT_LOGGER_FORMAT "[%TimeStamp%] <%Severity%> '%Process%': %Message%"

namespace te
{
  namespace core
  {
    class TECOREEXPORT Logger
    {

      public:

        /*!
          \brief It returns a reference to the singleton instance.

          \return A reference to the singleton instance.
         */
        static Logger& getInstance();

        /*!
          \brief It logs a given string message and severity

          \param message The string message to be logged
          \param severity The severity of the logged message
        */
        void log(const std::string& message, const std::string &channel, boost::log::trivial::severity_level severity);

        /*!
          \brief It sets the logger configuration from a given file

          \param filename The name of the configuration file

          \exception std::exception If the configuration file is doesn't load.
        */
        void addLoggerFromFile(const std::string &filename);

        /*!
          \brief It sets the logger using a default implementation

          \param filename The name of the log file.

          The logs will be stored in the given file name without any filters. If the file already
          exists, the logs will be appended to the end of the file.
        */
        void addLogger(const std::string &name, const std::string &filename, const std::string& format);

        /*!
          \brief It removes all added loggers.
         */
        void removeAllLoggers();

      private:

        /*! \brief Singleton constructor must be private. */
        Logger();

        /*! \brief Singleton destructor must be private. */
        ~Logger(){}

        /*! \brief Singleton copy constructor must be private. */
        Logger(Logger const&); // No copy allowed

        /*! \brief Singleton copy assignment operator must be private*/
        Logger& operator=(Logger const&); // No copy allowed

      private:

        /*! \brief Multi-Thread severity logger*/
        boost::log::sources::severity_channel_logger_mt<boost::log::trivial::severity_level, std::string>  m_logger;

        /*! \brief Logger file name */
        std::string m_filename = TERRALIB_DEFAULT_LOGGER;

        /*! \brief Logger message format */
        std::string m_format = TERRALIB_DEFAULT_LOGGER_FORMAT;

        /*! \brief Logger type status */
        bool m_fromfile = false;


    };
  }
}


/*!
  \def TE_ADD_LOGGER

  \brief Use this tag to init a logger using a default implementation without a configuration file.

  \param filename The name of the log file.
  */
#ifdef TERRALIB_LOGGER_ENABLED
  #define TE_ADD_LOGGER(name, filename, format) te::core::Logger::getInstance().addLogger(name, filename, format)
#else
  #define TE_ADD_LOGGER(filename) ((void)0)
#endif

/*!
  \def TE_INIT_LOGGER_FROM_FILE

  \brief Use this tag to init a logger using a configuration file.

  \param filename The name of the configuration file.

  \exception std::exception If the configuration file is doesn't load.
  */
#ifdef TERRALIB_LOGGER_ENABLED
  #define TE_ADD_LOGGER_FROM_FILE(filename) te::core::Logger::getInstance().addLoggerFromFile(filename)
#else
  #define TE_ADD  _LOGGER_FROM_FILE(filename) ((void)0)
#endif

/*!
  \def TE_LOG_TRACE

  \brief Use this tag in order to log a message to a specified logger with the TRACE level.

  \param msg The message to be logged.

  \note The TRACE Level designates finer-grained informational events than the DEBUG.
*/
#ifdef TERRALIB_LOGGER_ENABLED
  #define TE_LOG_CORE_TRACE(message) LOG_TRACE(message, TERRALIB_DEFAULT_LOGGER)
#else
  #define TE_LOG_CORE_TRACE(message) ((void)0)
#endif

/*!
  \def TE_LOG_FATAL

  \brief Use this tag in order to log a message to a specified logger with the ERROR level.

  \param msg The message to be logged.

  \note The ERROR level designates error events that might still allow the application to continue running.
*/
#ifdef TERRALIB_LOGGER_ENABLED
  #define TE_LOG_CORE_ERROR(message) LOG_ERROR(message, TERRALIB_DEFAULT_LOGGER)
#else
  #define TE_LOG_CORE_ERROR(message) ((void)0)
#endif

/*!
  \def TE_LOG_FATAL

  \brief Use this tag in order to log a message to a specified logger with the FATAL level.

  \param msg The message to be logged.

  \note The FATAL level designates very severe error events that will presumably lead the application to abort.
*/
#ifdef TERRALIB_LOGGER_ENABLED
  #define TE_LOG_CORE_FATAL(message) LOG_FATAL(message, TERRALIB_DEFAULT_LOGGER)
#else
  #define TE_LOG_CORE_FATAL(message) ((void)0)
#endif

/*!
  \def TE_LOG_INFO

  \brief Use this tag in order to log a message to a specified logger with the INFO level.

  \param msg The message to be logged.

  \note The INFO level designates informational messages that highlight the progress of the application at coarse-grained level.
*/
#ifdef TERRALIB_LOGGER_ENABLED
  #define TE_LOG_CORE_INFO(message) LOG_INFO(message, TERRALIB_DEFAULT_LOGGER)
#else
  #define TE_LOG_CORE_INFO(message) ((void)0)
#endif

/*!
  \def TE_LOG_WARN

  \brief Use this tag in order to log a message to a specified logger with the WARN level.

  \param msg The message to be logged.

  \note The WARN level designates potentially harmful situations.
*/
#ifdef TERRALIB_LOGGER_ENABLED
  #define TE_LOG_CORE_WARN(message) LOG_WARN(message, TERRALIB_DEFAULT_LOGGER)
#else
  #define TE_LOG_CORE_WARN(message) ((void)0)
#endif

/*!
  \def TE_LOG_DEBUG

  \brief Use this tag in order to log a message to a specified logger with the DEBUG level.

  \param msg The message to be logged.

  \note The DEBUG Level designates fine-grained informational events that are most useful to debug an application.
*/
#ifdef TERRALIB_LOGGER_ENABLED
  #define TE_LOG_CORE_DEBUG(message) LOG_DEBUG(message, TERRALIB_DEFAULT_LOGGER)
#else
  #define TE_LOG_CORE_DEBUG(message) ((void)0)
#endif

#define LOG_TRACE(message, channel) te::core::Logger::getInstance().log(message, channel ,boost::log::trivial::trace)

#define LOG_DEBUG(message, channel) te::core::Logger::getInstance().log(message, channel ,boost::log::trivial::debug)

#define LOG_INFO(message, channel) te::core::Logger::getInstance().log(message, channel ,boost::log::trivial::info)

#define LOG_WARN(message, channel) te::core::Logger::getInstance().log(message, channel ,boost::log::trivial::warning)

#define LOG_ERROR(message, channel) te::core::Logger::getInstance().log(message, channel ,boost::log::trivial::error)

#define LOG_FATAL(message, channel) te::core::Logger::getInstance().log(message, channel ,boost::log::trivial::fatal)

#endif  // __TERRALIB_LOGGER_LOGGER_H__
