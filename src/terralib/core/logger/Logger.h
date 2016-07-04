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

#ifndef __TERRALIB_CORE_LOGGER_LOGGER_H__
#define __TERRALIB_CORE_LOGGER_LOGGER_H__

// TerraLib
#include "../Config.h"
#include "../utils/Platform.h"
#include "../../BuildConfig.h"

// Boost
#include <boost/current_function.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/sources/severity_channel_logger.hpp>
#include <boost/log/utility/setup/file.hpp>

/*!
   \brief The default name of the log file if none is informed.
  */
const std::string TERRALIB_DEFAULT_LOGGER("terralib");

/*!
   \brief The default message format if none is informed.
  */
const std::string TERRALIB_DEFAULT_LOGGER_FORMAT("[%TimeStamp%] <%Severity%> %Message%");

namespace te
{
  namespace core
  {
    class TECOREEXPORT Logger
    {
      /*!
        \class Logger

        \brief This class is a singleton designed to manage log messages in TerraLib
       */
      public:

        /*!
          \brief It returns a reference to the singleton instance.

          \return A reference to the singleton instance.
         */
        static Logger& getInstance();

        /*!
          \brief It logs a given string message, channel and severity.

          \param message The string message to be logged.
          \param channel The channel name that will receive the message.
          \param severity The severity of the logged message.
        */
        void log(const std::string& message, const std::string &channel, boost::log::trivial::severity_level severity);

        /*!
          \brief It sets the logger configuration from a given file.

          \param filename The name of the configuration file.

          \exception std::exception If the configuration file is doesn't load.
        */
        void addLoggerFromFile(const std::string &filename);

        /*!
          \brief It sets the logger using a default implementation.

          \param name The name of the logger.
          \param filename The name of the log file.
          \param format The format string of the logger.

          \note The logs will be stored in the given file name. If the file already
          exists, the logs will be appended to the end of the file.
        */
        void addLogger(const std::string &name, const std::string &filename, const std::string& format);

        /*!
          \brief It removes all added loggers.
         */
        void removeAllLoggers();

      private:

        /*! \brief Singleton constructor must be private or protected. */
        Logger();

        /*! \brief Singleton destructor must be private or protected. */
        ~Logger(){}

        /*! \brief Singleton copy constructor must be private or protected. */
        Logger(Logger const&); // No copy allowed

        /*! \brief Singleton copy assignment operator must be private or protected. */
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
  } //end namespace core
}   //end namespace te


/*!
  \def TE_ADD_LOGGER

  \brief Use this tag to init a logger using a default implementation without a configuration file.

  \param name The name of the logger.
  \param filename The name of the log file.
  \param format The format string of the logger.
  */
#ifdef TERRALIB_LOGGER_ENABLED
  #define TE_ADD_LOGGER(name, filename, format) te::core::Logger::getInstance().addLogger(name, filename, format)
#else
  #define TE_ADD_LOGGER(name, filename, format) ((void)0)
#endif

/*!
  \def TE_ADD_LOGGER_FROM_FILE

  \brief Use this tag to init a logger using a configuration file.

  \param filename The name of the configuration file.

  \exception std::exception If the configuration file is doesn't load.
  */
#ifdef TERRALIB_LOGGER_ENABLED
  #define TE_ADD_LOGGER_FROM_FILE(filename) te::core::Logger::getInstance().addLoggerFromFile(filename)
#else
  #define TE_ADD_LOGGER_FROM_FILE(filename) ((void)0)
#endif

/*!
  \def TE_INIT_DEFAULT_LOGGER

  \brief Use this tag in order to initialize the default TerraLib logger.

  \param  filename The name of the log file.
 */
#ifdef TERRALIB_LOGGER_ENABLED
  #define TE_INIT_DEFAULT_LOGGER(filename) te::core::Logger::getInstance().addLogger(TERRALIB_DEFAULT_LOGGER, filename, TERRALIB_DEFAULT_LOGGER_FORMAT)
#else
  #define TE_INIT_DEFAULT_LOGGER(filename) ((void)0)
#endif

/*!
  \def TE_CORE_LOG_TRACE

  \brief Use this tag in order to log a message to a specified logger with the TRACE level.

  \param channel The name of your logger.
  \param message The message to be logged.
 */
#ifdef TERRALIB_LOGGER_TRACE_ENABLED
  #define TE_CORE_LOG_TRACE(channel, message) te::core::Logger::getInstance().log(message, channel ,boost::log::trivial::trace)
#else
  #define TE_CORE_LOG_TRACE(message) ((void)0)
#endif

/*!
  \def TE_CORE_LOG_DEBUG

  \brief Use this tag in order to log a message to a specified logger with the DEBUG level.

  \param channel The name of your logger.
  \param message The message to be logged.
 */
#ifdef TERRALIB_LOGGER_DEBUG_ENABLED
  #define TE_CORE_LOG_DEBUG(channel, message) te::core::Logger::getInstance().log(message, channel ,boost::log::trivial::debug)
#else
  #define TE_CORE_LOG_DEBUG(message) ((void)0)
#endif

/*!
  \def TE_CORE_LOG_INFO

  \brief Use this tag in order to log a message to a specified logger with the INFO level.

  \param channel The name of your logger.
  \param message The message to be logged.
 */
#ifdef TERRALIB_LOGGER_INFO_ENABLED
  #define TE_CORE_LOG_INFO(channel, message) te::core::Logger::getInstance().log(message, channel ,boost::log::trivial::info)
#else
  #define TE_CORE_LOG_INFO(message) ((void)0)
#endif

/*!
  \def TE_CORE_LOG_WARN

  \brief Use this tag in order to log a message to a specified logger with the WARN level.

  \param channel The name of your logger.
  \param message The message to be logged.
 */
#ifdef TERRALIB_LOGGER_WARN_ENABLED
  #define TE_CORE_LOG_WARN(channel, message) te::core::Logger::getInstance().log(message, channel ,boost::log::trivial::warning)
#else
  #define TE_CORE_LOG_WARN(message) ((void)0)
#endif

/*!
  \def TE_CORE_LOG_ERROR

  \brief Use this tag in order to log a message to a specified logger with the ERROR level.

  \param channel The name of your logger.
  \param message The message to be logged.
 */
#ifdef TERRALIB_LOGGER_ERROR_ENABLED
  #define TE_CORE_LOG_ERROR(channel, message) te::core::Logger::getInstance().log(message, channel ,boost::log::trivial::error)
#else
  #define TE_CORE_LOG_ERROR(message) ((void)0)
#endif

/*!
  \def TE_CORE_LOG_FATAL

  \brief Use this tag in order to log a message to a specified logger with the FATAL level.

  \param channel The name of your logger.
  \param message The message to be logged.
 */
#ifdef TERRALIB_LOGGER_FATAL_ENABLED
  #define TE_CORE_LOG_FATAL(channel, message) te::core::Logger::getInstance().log(message, channel ,boost::log::trivial::fatal)
#else
  #define TE_CORE_LOG_FATAL(message) ((void)0)
#endif

/*!
  \def TE_LOG_TRACE

  \brief Use this tag in order to log a message to the TerraLib default logger with the TRACE level.

  \param message The message to be logged.

  \note The TRACE Level designates finer-grained informational events than the DEBUG.
*/
#define TE_LOG_TRACE(message) TE_CORE_LOG_TRACE(TERRALIB_DEFAULT_LOGGER, std::string(BOOST_CURRENT_FUNCTION) + " : " + message)

/*!
  \def TE_LOG_DEBUG

  \brief Use this tag in order to log a message to the TerraLib default logger with the DEBUG level.

  \param message The message to be logged.

  \note The DEBUG Level designates fine-grained informational events that are most useful to debug an application.
*/
#define TE_LOG_DEBUG(message) TE_CORE_LOG_DEBUG(TERRALIB_DEFAULT_LOGGER, std::string(BOOST_CURRENT_FUNCTION) + " : " + message)

/*!
  \def TE_LOG_INFO

  \brief Use this tag in order to log a message to the TerraLib default logger with the INFO level.

  \param message The message to be logged.

  \note The INFO level designates informational messages that highlight the progress of the application at coarse-grained level.
*/
#define TE_LOG_INFO(message) TE_CORE_LOG_INFO(TERRALIB_DEFAULT_LOGGER, std::string(BOOST_CURRENT_FUNCTION) + " : " + message)

/*!
  \def TE_LOG_WARN

  \brief Use this tag in order to log a message to the TerraLib default logger with the WARN level.

  \param message The message to be logged.

  \note The WARN level designates potentially harmful situations.
*/
#define TE_LOG_WARN(message)TE_CORE_LOG_WARN(TERRALIB_DEFAULT_LOGGER, std::string(BOOST_CURRENT_FUNCTION) + " : " + message)

/*!
  \def TE_LOG_ERROR

  \brief Use this tag in order to log a message to the TerraLib default logger with the ERROR level.

  \param message The message to be logged.

  \note The ERROR level designates error events that might still allow the application to continue running.
*/
#define TE_LOG_ERROR(message) TE_CORE_LOG_ERROR(TERRALIB_DEFAULT_LOGGER, std::string(BOOST_CURRENT_FUNCTION) + " : " + message)

/*!
  \def TE_LOG_FATAL

  \brief Use this tag in order to log a message to the TerraLib default logger with the FATAL level.

  \param message The message to be logged.

  \note The FATAL level designates very severe error events that will presumably lead the application to abort.
*/
#define TE_LOG_FATAL(message) TE_CORE_LOG_FATAL(TERRALIB_DEFAULT_LOGGER, std::string(BOOST_CURRENT_FUNCTION) + " : " + message)

#endif  // __TERRALIB_CORE_LOGGER_LOGGER_H__
