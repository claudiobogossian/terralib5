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
#include <boost/log/utility/setup/file.hpp>

// TerraLib
#include "../../BuildConfig.h"
#include "Config.h"

/*!
  \def TERRALIB_DEFAULT_LOGGER

  \brief The default name of the log file if none is informed.
  */
#define TERRALIB_DEFAULT_LOGGER "terralib"

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
        void log(const std::string& message, boost::log::trivial::severity_level severity);

        /*!
          \brief It sets the logger configuration from a given file

          \param filename The name of the configuration file

          \exception std::exception If the configuration file is doesn't load.
        */
        void setupFromFile(const std::string &filename);

        /*!
          \brief It sets the logger using a default implementation

          \param filename The name of the log file.

          The logs will be stored in the given file name without any filters. If the file already
          exists, the logs will be appended to the end of the file.
        */
        void setupLogger(const std::string &filename);

        /*!
          \brief It sets a message format for the logger.

          \param format The string format for the logger

          \exception te::Exception If the logger was initialized from a configuration file
        */
        void setFormat(const std::string &format);

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
        boost::log::sources::severity_logger_mt<boost::log::trivial::severity_level>  m_logger;

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
  \def TE_INIT_LOGGER

  \brief Use this tag to init a logger using a default implementation without a configuration file.

  \param filename The name of the log file.
  */
#ifdef TERRALIB_LOGGER_ENABLED
  #define TE_INIT_LOGGER(filename) te::core::Logger::getInstance().setupLogger(filename)
#else
  #define TE_INIT_LOGGER(filename) ((void)0)
#endif

/*!
  \def TE_INIT_LOGGER_FROM_FILE

  \brief Use this tag to init a logger using a configuration file.

  \param filename The name of the configuration file.

  \exception std::exception If the configuration file is doesn't load.
  */
#ifdef TERRALIB_LOGGER_ENABLED
  #define TE_INIT_LOGGER_FROM_FILE(filename) te::core::Logger::getInstance().setupFromFile(filename)
#else
  #define TE_INIT_LOGGER_FROM_FILE(filename) ((void)0)
#endif

/*!
  \def TE_LOGGER_FORMAT

  \brief Use this tag to change the message format of the default logger.

  \param format The string format for the logger.
  */
#ifdef TERRALIB_LOGGER_ENABLED
  #define TE_LOGGER_FORMAT(format) te::core::Logger::getInstance().setFormat(format)
#else
  #define TE_LOGGER_FORMAT(format) ((void)0)
#endif

/*!
  \def TE_LOG_TRACE

  \brief Use this tag in order to log a message to a specified logger with the TRACE level.

  \param msg The message to be logged.

  \note The TRACE Level designates finer-grained informational events than the DEBUG.
*/
#ifdef TERRALIB_LOGGER_ENABLED
  #define TE_LOG_CORE_TRACE(message) te::core::Logger::getInstance().log(message, boost::log::trivial::trace)
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
  #define TE_LOG_CORE_ERROR(message) te::core::Logger::getInstance().log(message, boost::log::trivial::error)
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
  #define TE_LOG_CORE_FATAL(message) te::core::Logger::getInstance().log(message, boost::log::trivial::fatal)
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
  #define TE_LOG_CORE_INFO(message) te::core::Logger::getInstance().log(message, boost::log::trivial::info)
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
  #define TE_LOG_CORE_WARN(message) te::core::Logger::getInstance().log(message, boost::log::trivial::warning)
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
  #define TE_LOG_CORE_DEBUG(message) te::core::Logger::getInstance().log(message, boost::log::trivial::debug)
#else
  #define TE_LOG_CORE_DEBUG(message) ((void)0)
#endif



#endif  // __TERRALIB_LOGGER_LOGGER_H__
