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
  \file terralib/core/logger/Logger.h

  \brief This class is designed to create Logs in TerraLib.

  \author Matheus Cavassan Zaglia
  \author Gilberto Ribeiro de Queiroz
*/

#ifndef __TERRALIB_LOGGER_TRANSLATOR_H__
#define __TERRALIB_LOGGER_TRANSLATOR_H__

// Boost
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/sources/severity_channel_logger.hpp>

// TerraLib
#include "../../BuildConfig.h"
#include "Config.h"

// STL
#include <string>

namespace te
{
  namespace core
  {

    class TECOREEXPORT Logger
    {

      public:

        Logger(const std::string &filename);
        ~Logger();
        void logInfo(const std::string& message);
        void logTrace(const std::string& message);
        void logWarning(const std::string& message);
        void logError(const std::string& message);
        void logFatal(const std::string& message);
        void logDebug(const std::string& message);

      protected:
          boost::log::sources::severity_channel_logger_mt<boost::log::trivial::severity_level, std::string>  m_logger;

    };

  } // end namespace common
}   // end namespace te
#define TE_LOG_INIT(name) te::core::Logger logger(name)

#define TE_LOG_TRACE(message)   logger.logTrace(message)
#define TE_LOG_ERROR(message)   logger.logError(message)
#define TE_LOG_FATAL(message)   logger.logFatal(message)
#define TE_LOG_INFO(message)    logger.logInfo(message)
#define TE_LOG_WARNING(message) logger.logWarning(message)
#define TE_LOG_DEBUG(message)   logger.logDebug(message)


#endif  // __TERRALIB_LOGGER_TRANSLATOR_H__
