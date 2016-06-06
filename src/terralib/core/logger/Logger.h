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
#include <boost/log/trivial.hpp>
#include <boost/log/sources/global_logger_storage.hpp>

// TerraLib
#include "../../BuildConfig.h"
#include "Config.h"

BOOST_LOG_GLOBAL_LOGGER(logger, boost::log::sources::severity_logger_mt<boost::log::trivial::severity_level>)

namespace te
{
  namespace core
  {
    class Logger
    {
      public:
        static void log(const std::string& message, boost::log::trivial::severity_level severity);
    };
  }
}


#define TE_LOG_CORE_TRACE(message)   te::core::Logger::log(message, boost::log::trivial::trace)
#define TE_LOG_CORE_ERROR(message)   te::core::Logger::log(message, boost::log::trivial::error)
#define TE_LOG_CORE_FATAL(message)   te::core::Logger::log(message, boost::log::trivial::fatal)
#define TE_LOG_CORE_INFO(message)    te::core::Logger::log(message, boost::log::trivial::info)
#define TE_LOG_CORE_WARNING(message) te::core::Logger::log(message, boost::log::trivial::warning)
#define TE_LOG_CORE_DEBUG(message)   te::core::Logger::log(message, boost::log::trivial::debug)


#endif  // __TERRALIB_LOGGER_TRANSLATOR_H__
