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
  \file terralib/common/Logger.h

  \brief This class is designed to manage the log of information in TerraLib.
 */

#ifndef __TERRALIB_COMMON_INTERNAL_LOGGER_H
#define __TERRALIB_COMMON_INTERNAL_LOGGER_H

// TerraLib
#include "Config.h"


/*!
  \def TE_LOG_FATAL
  
  \brief Use this tag in order to log a message to a specified logger with the FATAL level.

  \param msg The message to be logged. Example: "Exception raised because of a missing parameter!".

  \note The FATAL level designates very severe error events that will presumably lead the application to abort. 
*/
#if defined(TERRALIB_LOGGER_ENABLED) && defined(TERRALIB_LOGGER_FATAL_ENABLED)
  #define TE_LOG_FATAL(msg) te::common::Logger::logFatal(TERRALIB_LOGGER_DEFAULT_NAME, msg)
#else
  #define TE_LOG_FATAL(msg) ((void)0)
#endif

/*!
  \def TE_LOG_ASSERT
  
  \brief Use this tag in order to log a message to a specified logger with the ASSERT level.

  \param condition An expression (condition). If it is not true, the message will be logged.
  \param msg       The message to be logged. Example: "Exception raised because of a missing parameter!".

  \note The ASSERT level can be used to check expressions that must be evaluated as true.
*/
#if defined(TERRALIB_LOGGER_ENABLED) && defined(TERRALIB_LOGGER_ASSERT_ENABLED)
  #define TE_LOG_ASSERT(condition, msg) te::common::Logger::logAssert(TERRALIB_LOGGER_DEFAULT_NAME, condition, msg)
#else
  #define TE_LOG_ASSERT(condition, msg) ((void)0)
#endif

/*!
  \def TE_LOG_ERROR
  
  \brief Use this tag in order to log a message to a specified logger with the ERROR level.

  \param msg The message to be logged. Example: "Exception raised because of a missing parameter!".

  \note The ERROR level designates error events that might still allow the application to continue running.
*/
#if defined(TERRALIB_LOGGER_ENABLED) && defined(TERRALIB_LOGGER_ERROR_ENABLED)
  #define TE_LOG_ERROR(msg) te::common::Logger::logError(TERRALIB_LOGGER_DEFAULT_NAME, msg)
#else
  #define TE_LOG_ERROR(msg) ((void)0)
#endif

/*!
  \def TE_LOG_WARN
  
  \brief Use this tag in order to log a message to a specified logger with the WARN level.

  \param msg The message to be logged. Example: "Exception raised because of a missing parameter!".

  \note The WARN level designates potentially harmful situations.
*/
#if defined(TERRALIB_LOGGER_ENABLED) && defined(TERRALIB_LOGGER_WARN_ENABLED)
  #define TE_LOG_WARN(msg) te::common::Logger::logWarning(TERRALIB_LOGGER_DEFAULT_NAME, msg)
#else
  #define TE_LOG_WARN(msg) ((void)0)
#endif

/*!
  \def TE_LOG_INFO
  
  \brief Use this tag in order to log a message to a specified logger with the INFO level.

  \param msg The message to be logged. Example: "Exception raised because of a missing parameter!".

  \note The INFO level designates informational messages that highlight the progress of the application at coarse-grained level.
*/
#if defined(TERRALIB_LOGGER_ENABLED) && defined(TERRALIB_LOGGER_INFO_ENABLED)
  #define TE_LOG_INFO(msg) te::common::Logger::logInfo(TERRALIB_LOGGER_DEFAULT_NAME, msg)
#else
  #define TE_LOG_INFO(msg) ((void)0)
#endif

/*!
  \def TE_LOG_DEBUG
  
  \brief Use this tag in order to log a message to a specified logger with the DEBUG level.

  \param msg  The message to be logged. Example: "Exception raised because of a missing parameter!".

  \note The DEBUG Level designates fine-grained informational events that are most useful to debug an application.
*/
#if defined(TERRALIB_LOGGER_ENABLED) && defined(TERRALIB_LOGGER_DEBUG_ENABLED)
  #define TE_LOG_DEBUG(msg) te::common::Logger::logDebug(TERRALIB_LOGGER_DEFAULT_NAME, msg)
#else
  #define TE_LOG_DEBUG(msg) ((void)0)
#endif

/*!
  \def TE_LOG_TRACE
  
  \brief Use this tag in order to log a message to a specified logger with the TRACE level.

  \param msg The message to be logged. Example: "Exception raised because of a missing parameter!".

  \note The TRACE Level designates finer-grained informational events than the DEBUG.
*/
#if defined(TERRALIB_LOGGER_ENABLED) && defined(TERRALIB_LOGGER_TRACE_ENABLED)
  #define TE_LOG_TRACE(msg) te::common::Logger::logTrace(TERRALIB_LOGGER_DEFAULT_NAME, msg)
#else
  #define TE_LOG_TRACE(msg) ((void)0)
#endif

/*!
  \def TERRALIB_LOGGER_DEFAULT_NAME
  
  \brief This is the fully qualified TerraLib root logger.
  
  \note If you are developing a new module and want to have your
        own log configuration file, please give it a name starting with "terralib.your-module-name". So it will
        belong to TerraLib loggers tree.
*/
#define TERRALIB_LOGGER_DEFAULT_NAME "terralib"

/*!
  \def TERRALIB_LOGGER_DEFAULT_CONFIG_FILE
  
  \brief If logger configuration is file based (text or XML), the file name (with its full path or relative path to TerraLib current directory).
*/
#define TERRALIB_LOGGER_DEFAULT_CONFIGURATION_FILE "conf/te-log.conf"

/*!
  \def TERRALIB_LOGGER_DEFAULT_CONFIG_FILE_TYPE
  
  \brief It sets the default type of file used to configure the logger.
  
  \note See LoggerConfigurationType enum for more information about possible values for this macro.
*/
#define TERRALIB_LOGGER_DEFAULT_CONFIG_FILE_TYPE te::common::LOGGER_TXT_CONFIG

/*!
  \def TERRALIB_LOGGER_MAKE_DEFAULT_INITIALIZATION

  \brief This macro can be used to make logger default initialization.
*/
#ifdef TERRALIB_LOGGER_ENABLED
  #define TERRALIB_LOGGER_MAKE_DEFAULT_INITIALIZATION() \
            const std::string loggerConfFile = te::common::Logger::getDefaultConfigFile();  \
            te::common::Logger::initialize(TERRALIB_LOGGER_DEFAULT_NAME, TERRALIB_LOGGER_DEFAULT_CONFIG_FILE_TYPE, loggerConfFile);
#else
  #define TERRALIB_LOGGER_MAKE_DEFAULT_INITIALIZATION() ((void)0)
#endif

/*!
  \def TERRALIB_LOGGER_MAKE_DEFAULT_FINALIZATION

  \brief This macro can be used to make logger default finalization.
*/
#ifdef TERRALIB_LOGGER_ENABLED
  #define TERRALIB_LOGGER_MAKE_DEFAULT_FINALIZATION() te::common::Logger::finalize(TERRALIB_LOGGER_DEFAULT_NAME)
#else
  #define TERRALIB_LOGGER_MAKE_DEFAULT_FINALIZATION() ((void)0)
#endif

#ifdef TERRALIB_LOGGER_ENABLED

// TerraLib
#include "Enums.h"
#include "Static.h"

// STL
#include <string>

namespace te
{
  namespace common
  {
    /*!
      \class Logger
      
      \brief This class is designed to manage the log of information in TerraLib.
      
      You just need to register the name of your logger
      and the configuration file and type used by it.
      When ready, you call initLogger for setting up your logger.

      All loggers name should start with "terralib." in order to be
      in the TerraLib logger tree. So, if you have an extension called Common,
      the name of your logger could be "terralib.common". If the name
      was WMS, you could use "terralib.wms" and so on.

      The use of log in TerraLib doesn't need to call methods of this class,
      instead just use the macros to report to the log. It will allow you to turn the log off
      when desired.

      Don't worry! Logging is used in TerraLib with so much care...
      only in special places, in order to not slowdown its
      performance. So we don't have any performance problem/penality by using a log.
      And if you want, you can disable the logger so that no code is put into
      the executable, see TERRALIB_LOGGER_ENABLED.

      \note The methods provided by this class must be used only after all static data has been initialized.

      \ingroup common
    */
    class TECOMMONEXPORT Logger : public Static
    {
      public:

        /** @name Configuration Methods
         *  Methods used to configure, initialize and finalize the logger.
         */
        //@{

        /*!
          \brief It returns the default configuration file name with full path.

          If none is set this method will search for a file defined in the macro TERRALIB_LOGGER_DEFAULT_CONFIGURATION_FILE (conf/te-log.conf):
          <ul>
          <li>under application default dir</li>
          <li>then in the dir specified by the TERRALIB_DIR_ENVIRONMENT_VARIABLE</li>
          </ul>

          If no file is found they return an empty string.

          \return The default configuration file name with full path or an empty string if none is found.
         */
        static std::string getDefaultConfigFile();

        /*!
          \brief It initializes a given logger based on its configuration properties.

          By default, if you not specify a logger configuration file, your new logger
          will share the same configuration as the TerraLib tree of loggers.

          \param loggerName The fully qualified logger name. Example: terralib.common.
          \param t          The type of configuration to be used by the logger.
          \param fileName   If logger configuration is file based (text or XML), the file name (with its full path or the relative path to the TerraLib current directory).

          \exception Exception It raises an exception if the fileName is missing for a file based configuraton logger.
         */
        static void initialize(const std::string& loggerName,
                               const LoggerConfigurationType t,
                               const std::string& fileName);

        /*!
          \brief It initializes a given logger based on its parent configuration.
          
          \param loggerName The fully qualified logger name. Example: terralib.common.
          
          \exception Exception It raises an exception if the fileName is missing.
         */
        static void initialize(const std::string& loggerName);

        /*!
          \brief It removes the appenders and other stuffs from the logger and then eliminates them from the system.
                    
          \param loggerName The fully qualified logger name. Example: terralib.common.
         */
        static void finalize(const std::string& loggerName);

        //@}

        /** @name Logging Methods
         *  Methods that can be used to send messages to the log. Please, see the logger macros defined in Config.h.
         *  There are a set of macros that can me used instead of using the methods of this class that can
         *  make things easier when disabling the logger support.
         */
        //@{

        /*!
          \brief Use this class method in order to log a message to a specified logger with the FATAL level.

          \param logger The name of a logger. Example: "te.common".
          \param msg    The message to be logged. Example: "Exception raised because of a missing parameter!".

          \note The FATAL level designates very severe error events that will presumably lead the application to abort.

          \note Avoid using this method. Try to use the macro TE_LOG_FATAL or TE_TASK_LOG_FATAL.
         */
        static void logFatal(const char* logger, const char* msg);

        /*!
          \brief Use this class method in order to log a message to a specified logger with the FATAL level.

          \param logger The name of a logger. Example: "te.common".
          \param msg    The message to be logged. Example: "Exception raised because of a missing parameter!".

          \note The FATAL level designates very severe error events that will presumably lead the application to abort.

          \note Avoid using this method. Try to use the macro TE_LOG_FATAL or TE_TASK_LOG_FATAL.
         */
        static void logFatal(const std::string& logger, const std::string& msg);

        /*!
          \brief Use this class method in order to log a message to a specified logger with the ASSERT level.

          \param logger    The name of a logger. Example: "te.common".
          \param condition An expression (condition). If it is not true, the message will be logged.
          \param msg       The message to be logged. Example: "Exception raised because of a missing parameter!".

          \note The ASSERT level can be used to check expressions that must be evaluated as true.

          \note Avoid using this method. Try to use the macro TE_LOG_ASSERT or TE_TASK_LOG_ASSERT.
         */
        static void logAssert(const char* logger, bool condition, const char* msg);

        /*!
          \brief Use this class method in order to log a message to a specified logger with the ERROR level.

          \param logger The name of a logger. Example: "te.common".
          \param msg    The message to be logged. Example: "Exception raised because of a missing parameter!".

          \note The ERROR level designates error events that might still allow the application to continue running.

          \note Avoid using this method. Try to use the macro TE_LOG_ERROR or TE_TASK_LOG_ERROR.
         */
        static void logError(const char* logger, const char* msg);

        /*!
          \brief Use this class method in order to log a message to a specified logger with the WARNING level.

          \param logger The name of a logger. Example: "te.common".
          \param msg    The message to be logged. Example: "Exception raised because of a missing parameter!".

          \note The WARNING level designates potentially harmful situations.

          \note Avoid using this method. Try to use the macro TE_LOG_WARNING or TE_TASK_LOG_WARNING.
         */
        static void logWarning(const char* logger, const char* msg);

        /*!
          \brief Use this class method in order to log a message to a specified logger with the INFO level.

          \param logger The name of a logger. Example: "te.common".
          \param msg    The message to be logged. Example: "Exception raised because of missing parameter!".

          \note The INFO level designates informational messages that highlight the progress of the application at coarse-grained level. 

          \note Avoid using this method. Try to use the macro TE_LOG_INFO or TE_TASK_LOG_INFO.
         */
        static void logInfo(const char* logger, const char* msg);

        /*!
          \brief Use this class method in order to log a message to a specified logger with the INFO level.

          \param logger The name of a logger. Example: "te.common".
          \param msg    The message to be logged. Example: "Exception raised because of missing parameter!".

          \note The INFO level designates informational messages that highlight the progress of the application at coarse-grained level. 

          \note Avoid using this method. Try to use the macro TE_LOG_INFO or TE_TASK_LOG_INFO.
         */
        static void logInfo(const std::string& logger, const std::string& msg);

        /*!
          \brief Use this clas smethod in order to log a message to a specified logger with the DEBUG level.

          \param logger The name of a logger. Example: "te.common".
          \param msg    The message to be logged. Example: "Exception raised because of missing parameter!".

          \note The DEBUG Level designates fine-grained informational events that are most useful to debug an application.

          \note Avoid using this method. Try to use the macro TE_LOG_DEBUG or TE_TASK_LOG_DEBUG.
         */
        static void logDebug(const char* logger, const char* msg);

        /*!
          \brief Use this class method in order to log a message to a specified logger with the TRACE level.

          \param logger The name of a logger. Example: "terralib".
          \param msg    The message to be logged. Example: "Exception raised because of missing parameter!".

          \note The TRACE Level designates finer-grained informational events than the DEBUG.

          \note Avoid using this method. Try to use the macro TE_LOG_TRACE or TE_TASK_LOG_TRACE.
         */
        static void logTrace(const char* logger, const char* msg);

        /*!
          \brief Use this class method in order to log a message to a specified logger with the TRACE level.

          \param logger The name of a logger. Example: "terralib".
          \param msg    The message to be logged. Example: "Exception raised because of missing parameter!".

          \note The TRACE Level designates finer-grained informational events than the DEBUG.

          \note Avoid using this method. Try to use the macro TE_LOG_TRACE or TE_TASK_LOG_TRACE.
         */
        static void logTrace(const std::string& logger, const std::string& msg);

        //@}

      private:

        /** @name Instantiation of Objects
         *  Instantiation of objects don't allowed.
         */
        //@{

        /*! \brief As all private constructor, it doesn't allow direct instantiation of a Logger. */
        //Logger();

        //@}
    };

  } // end namespace common
}   // end namespace te

#endif  // TERRALIB_LOGGER_ENABLED

#endif  // __TERRALIB_COMMON_INTERNAL_LOGGER_H

