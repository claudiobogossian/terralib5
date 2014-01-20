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
  \file terralib/common/Config.h

  \brief Configuration flags for the TerraLib Common Runtime module.

  \todo We need to move some configuration variables to build system.
*/

#ifndef __TERRALIB_COMMON_INTERNAL_CONFIG_H
#define __TERRALIB_COMMON_INTERNAL_CONFIG_H

// TerraLib
#include "../Config.h"

#define TE_COMMON_MODULE_NAME "te.common"

/** @name File Defines
 *  Flags for TerraLib special files name and location.
 */
//@{

/*!
  \def TERRALIB_DIR

  \brief The folder name on the system where terralib data is stored (on windows: app/terralib).
*/
#ifndef TERRALIB_DIR
  #define TERRALIB_DIR     "terralib"
#endif

/*!
  \def TERRALIB_DIR_ENVVAR

  \brief Environment variable name with the TerraLib install path.
*/
#ifndef TERRALIB_DIR_ENVVAR
  #define TERRALIB_DIR_ENVVAR   "TERRALIB_DIR"
#endif

/*!
  \def TERRALIB_CONFIG_DIR

  \brief Folder location with application config files.
*/
#ifndef TERRALIB_CONFIG_DIR
  #define TERRALIB_CONFIG_DIR    "../../conf"
#endif

/*!
  \def TERRALIB_CONFIG_DIR

  \brief Main application config file.
*/
#ifndef TERRALIB_SYSTEM_SETTINGS_FILE
  #define TERRALIB_SYSTEM_SETTINGS_FILE       "config.xml"
#endif

/*!
  \def TERRALIB_USER_SETTINGS_FILE

  \brief User settings file name
*/
#ifndef TERRALIB_USER_SETTINGS_FILE
  #define TERRALIB_USER_SETTINGS_FILE         "user_settings.xml"
#endif

//@}

/** @name Internationalization Defines
 *  Flags for TerraLib code internationalization.
 */
//@{

// Check if the TR macro has already been defined
// by another application... if so, it will output
// an error message and stop compiling!
#if TE_TRANSLATOR_ENABLED
  #ifdef TR
    #error "The TR macro has been already defined by another application or code. Please, inform TerraLib Development Team <developers@terralib.org>, we will be glad to help solving this problem!"
  #endif
#endif

/*!
  \def TE_ADD_TEXT_DOMAIN

  \brief It adds the given text domain located at domain-dir with the given codeset to the multilingual system.

  \note This macro will check if the domain already exists before doing anyting.
*/
#if TE_TRANSLATOR_ENABLED
  #define TE_ADD_TEXT_DOMAIN(domain, domaindir, codeset) \
            if(!te::common::Translator::getInstance().exist(domain))                       \
            {                                                                              \
              te::common::Translator::getInstance().addTextDomain(domain, domaindir);      \
              te::common::Translator::getInstance().setTextDomainCodeSet(domain, codeset); \
            }                                                                              \
            ((void)0)
#else
  #define TE_ADD_TEXT_DOMAIN(domain, domaindir, codeset) ((void)0)
#endif

/*!
  \def TR

  \brief Try to translate the message according to the given domain. See the TR_COMMON macro for more infomation on how to create a translation mark for your code.
*/
#if TE_TRANSLATOR_ENABLED
  #define TR(message, domain) te::common::Translator::getInstance().getInstance().translate(message, domain)
#else
  #define TR(message, domain) message
#endif

/*!
  \def TR_PLURAL

  \brief Try to translate the message according to the given domain and plural form. See the TR_PLURAL_COMMON macro for more infomation on how to create a translation mark for your code.
*/
#if TE_TRANSLATOR_ENABLED
  #define TR_PLURAL(domain, message1, message2, n) te::common::Translator::getInstance().getInstance().translate(domain, message1, message2, n)
#else
  #define TR_PLURAL(domain, message1, message2, n) (n > 1 ? message2 : message1)
#endif

/*!
  \def TE_COMMON_TEXT_DOMAIN

  \brief It contains the name of the text domain used in the translation of messages in the TerraLib Common module.
*/
#define TE_COMMON_TEXT_DOMAIN "tecommon"

/*!
  \def TE_COMMON_TEXT_DOMAIN_DIR

  \brief It contains the translation catalog directory.
*/
#define TE_COMMON_TEXT_DOMAIN_DIR "locale"

/*!
  \def TR_COMMON

  \brief It marks a string in order to get translated. This is the mark used in the Common module of TerraLib. 

  In each module you have to define a new mark. It can be hard-working,
  but it gives you the maximum flexibility to define your own text domain
  in the apropriated directory.
  <br>
  Example of usage:
  \code
  std::cout << TR_COMMON("My message!");

  throw Exception(TR_COMMON("My other message!"));
  \endcode
*/
#define TR_COMMON(message) TR(message, TE_COMMON_TEXT_DOMAIN)

/*!
  \def TR_PLURAL_COMMON

  \brief It marks a string in order to get translated according to plural form. This is the mark used in the Common module of TerraLib. 

  In each module you have to define a new mark. It can be hard-working,
  but it gives you the maximum flexibility to define your own text domain
  in the apropriated directory.
  <br>
  Example of usage:
  \code
  int n = f(...);

  std::cout << TR_PLURAL_COMMON("One Message!", "Two Messages", n);

  throw Exception(TR_PLURAL_COMMON("One Message!", "Two Messages", n));
  \endcode

  In the above example, the parameter n can be
  a threshold that helps to choose between the first or the second construction.
  If your trabslation file is configured with a theashold of 1,
  indicating that if n > 1 must choose the second construction,
  the plural versin will be choosed, otherwise, it will choose the
  singular form (the fisrt one).
*/
#define TR_PLURAL_COMMON(message1, message2, n) TR_PLURAL(TE_COMMON_TEXT_DOMAIN, message1, message2, n)

//@}

/** @name Logger Defines
 *  Flags for building TerraLib with log support.
 *
 *  The macros TE_LOG_XXX can be used to log messages.
 *  These macros are disabled if TE_LOGGER_ENABLED
 *  is set to 0.
 *
 *  Remember to include the <common/Logger.h> file when using this set of macros.
 */
//@{

/*!
  \def TE_LOGGER_DEFAULT_NAME
  
  \brief This is the fully qualified TerraLib root logger.
  
  \note If you are developing a new module and want to have your
        own log configuration file, please give it a name starting with "terralib.your-module-name". So it will
        belong to TerraLib loggers tree.
*/
#define TE_LOGGER_DEFAULT_NAME "terralib"

/*!
  \def TE_LOGGER_DEFAULT_CONFIG_FILE
  
  \brief If logger configuration is file based (text or XML), the file name (with its full path or relative path to TerraLib current directory).
*/
#define TE_LOGGER_DEFAULT_CONFIGURATION_FILE "conf/te-log.conf"

/*!
  \def TE_LOGGER_DEFAULT_CONFIG_FILE_TYPE
  
  \brief It sets the default type of file used to configure the logger.
  
  \note See LoggerConfigurationType enum for more information about possible values for this macro.
*/
#define TE_LOGGER_DEFAULT_CONFIG_FILE_TYPE te::common::LOGGER_TXT_CONFIG

/*!
  \def TE_LOGGER_MAKE_DEFAULT_INITIALIZATION

  \brief This macro can be used to make logger default initialization.
*/
#if TE_LOGGER_ENABLED
  #define TE_LOGGER_MAKE_DEFAULT_INITIALIZATION() \
            const std::string loggerConfFile = te::common::Logger::getDefaultConfigFile();  \
            te::common::Logger::initialize(TE_LOGGER_DEFAULT_NAME, TE_LOGGER_DEFAULT_CONFIG_FILE_TYPE, loggerConfFile);
#else
  #define TE_LOGGER_MAKE_DEFAULT_INITIALIZATION() ((void)0)
#endif

/*!
  \def TE_LOGGER_MAKE_DEFAULT_FINALIZATION

  \brief This macro can be used to make logger default finalization.
*/
#if TE_LOGGER_ENABLED
  #define TE_LOGGER_MAKE_DEFAULT_FINALIZATION() te::common::Logger::finalize(TE_LOGGER_DEFAULT_NAME)
#else
  #define TE_LOGGER_MAKE_DEFAULT_FINALIZATION() ((void)0)
#endif

/*!
  \def TE_LOGGER_FATAL_ENABLED
  
  \brief If this flag is set to 0 the macro TE_LOG_FATAL will be disabled and no additional computational costs will be added to the code.
*/
#define TE_LOGGER_FATAL_ENABLED 1

/*!
  \def TE_LOGGER_ASSERT_ENABLED
  
  \brief If this flag is set to 0 the assertion macro TE_LOG_ASSERT will be disabled and no additional computational costs will be added to the code.
*/
#define TE_LOGGER_ASSERT_ENABLED 1

/*!
  \def TE_LOGGER_ERROR_ENABLED
  
  \brief If this flag is set to 0 the assertion macro TE_LOG_ERROR will be disabled and no additional computational costs will be added to the code.
*/
#define TE_LOGGER_ERROR_ENABLED 1

/*!
  \def TE_LOGGER_WARN_ENABLED
  
  \brief If this flag is set to 0 the assertion macro TE_LOG_WARN will be disabled and no additional computational costs will be added to the code.
*/
#define TE_LOGGER_WARN_ENABLED 1

/*!
  \def TE_LOGGER_INFO_ENABLED
  
  \brief If this flag is set to 0 the assertion macro TE_LOG_INFO will be disabled and no additional computational costs will be added to the code.
*/
#define TE_LOGGER_INFO_ENABLED 1

/*!
  \def TE_LOGGER_DEBUG_ENABLED
  
  \brief If this flag is set to 0 the assertion macro TE_LOG_DEBUG will be disabled and no additional computational costs will be added to the code.
*/
#define TE_LOGGER_DEBUG_ENABLED 1

/*!
  \def TE_LOGGER_TRACE_ENABLED
  
  \brief If this flag is set to 0 the assertion macro TE_LOG_TRACE will be disabled and no additional computational costs will be added to the code.
*/
#define TE_LOGGER_TRACE_ENABLED 1


//@}

#define TE_PROTOCOL_DEFAULT_PORTS_FILE "protocol/protocolPorts.txt"

/** @name DLL/LIB Module
 *  Flags for building TerraLib as a DLL or as a Static Library
 */
//@{

/*!
  \def TECOMMONEXPORT

  \brief You can use this macro in order to export/import classes and functions from this module.

  \note If you want to compile TerraLib as DLL in Windows, remember to insert TECOMMONDLL into the project's list of defines.

  \note If you want to compile TerraLib as an Static Library under Windows, remember to insert the TECOCOMMONSTATIC flag into the project list of defines.
*/
#ifdef WIN32

  #ifdef _MSC_VER 
    #pragma warning( disable : 4251 )
    #pragma warning( disable : 4275 )
    #pragma warning( disable : 4290 )
  #endif

  #ifdef TECOMMONSTATIC
    #define TECOMMONEXPORT                          // Don't need to export/import... it is a static library
  #elif TECOMMONDLL
    #define TECOMMONEXPORT  __declspec(dllexport)   // export DLL information
  #else
    #define TECOMMONEXPORT  __declspec(dllimport)   // import DLL information
  #endif 
#else
  #define TECOMMONEXPORT
#endif

//@}

#endif  // __TERRALIB_COMMON_INTERNAL_CONFIG_H

