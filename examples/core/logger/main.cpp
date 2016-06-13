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

int main(int argc, char *argv[])
{
//Starting a logger from a configuration file.
  TE_INIT_LOGGER_FROM_FILE(te::core::FindInTerraLibPath("share/terralib/config/te-log.ini"));
  TE_LOG_CORE_TRACE("This is a trace log.");
  TE_LOG_CORE_INFO("This is a info log");
  TE_LOG_CORE_ERROR("This is a error log");

  //Initializing a log using a default configuration.
  //When initializing a new log, the previous one will not log anymore.
  TE_INIT_LOGGER("logs/mylogs.log");
  TE_LOG_CORE_WARN("This is a warning log.");
  TE_LOGGER_FORMAT("[%TimeStamp%]{%ThreadID%} %Process%(%ProcessID%) <%Severity%>: %Message%");
  TE_LOG_CORE_FATAL("This is a fatal log.");
  TE_LOG_CORE_DEBUG("This is a debug log.");
}
