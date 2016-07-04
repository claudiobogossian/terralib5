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
  \file terralib/examples/core/logger/main.cpp

  \brief Examples for the Terralib Logger Module

  \author Matheus Cavassan Zaglia
*/

// TerraLib
#include <terralib/core/utils/Platform.h>
#include <terralib/core/logger/Logger.h>

#define MY_LOG_WARN(message) TE_CORE_LOG_WARN("mylogger", message)
#define MY_LOG_FATAL(message) TE_CORE_LOG_FATAL("mylogger", message)
#define MY_LOG_DEBUG(message) TE_CORE_LOG_DEBUG("mylogger", message)

int main(int argc, char *argv[])
{

  //Initializing terralib default logger
  TE_INIT_DEFAULT_LOGGER("logs/terralib.log");

  //Adding a logger from a configuration file.
  TE_ADD_LOGGER_FROM_FILE(te::core::FindInTerraLibPath("share/terralib/config/te-log.ini"));

  TE_LOG_TRACE("This is a trace log.");
  TE_CORE_LOG_DEBUG("vp", "Logging config file");
  TE_LOG_INFO("This is a info log");
  TE_CORE_LOG_DEBUG("vp", "Logging config file");
  TE_CORE_LOG_DEBUG("mnt", "Logging config file");
  TE_LOG_ERROR("This is a error log");
  TE_CORE_LOG_DEBUG("attributefill", "Logging config file");

  //Adding a new logger without configuration file.
  TE_ADD_LOGGER("mylogger", "log/mylogs.log", "[%TimeStamp%]{%ThreadID%} %Process%(%ProcessID%) <%Severity%>: %Message%");

  MY_LOG_WARN("This is a warning log.");
  MY_LOG_FATAL("This is a fatal log.");
  TE_LOG_INFO("This is a info log");
  TE_LOG_ERROR("This is a error log");
  MY_LOG_DEBUG("This is a debug log.");
}
