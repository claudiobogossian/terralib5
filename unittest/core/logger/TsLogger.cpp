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
  \file terralib/unittest/core/logger/TsLogger.cpp

  \brief A test suit for the TerraLib Core Logger Module.

  \author Matheus Cavassan Zaglia.
 */

// TerraLib
#include <terralib/core/logger/Logger.h>
#include <terralib/core/utils/Platform.h>

// Boost
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(logger_test_case)

BOOST_AUTO_TEST_CASE(default_logger_test)
{
  BOOST_CHECK_NO_THROW(TE_INIT_LOGGER("logs/mylogs.log"));

  BOOST_CHECK_NO_THROW(TE_LOG_CORE_WARN("Warning log"));
  BOOST_CHECK_NO_THROW(TE_LOG_CORE_INFO("Info log"));
  BOOST_CHECK_NO_THROW(TE_LOG_CORE_ERROR("Error log"));
  BOOST_CHECK_NO_THROW(TE_LOG_CORE_TRACE("Trace log"));
  BOOST_CHECK_NO_THROW(TE_LOG_CORE_FATAL("Fatal log"));
  BOOST_CHECK_NO_THROW(TE_LOG_CORE_DEBUG("Debug log"));
}

BOOST_AUTO_TEST_CASE(configuration_file_logger_test)
{
  BOOST_CHECK_THROW(TE_INIT_LOGGER_FROM_FILE("mylogger.ini"), std::exception);
  BOOST_CHECK_NO_THROW(TE_INIT_LOGGER_FROM_FILE(te::core::FindInTerraLibPath("share/terralib/config/te-log.ini")));

  BOOST_CHECK_NO_THROW(TE_LOG_CORE_WARN("Warning log"));
  BOOST_CHECK_NO_THROW(TE_LOG_CORE_INFO("Info log"));
  BOOST_CHECK_NO_THROW(TE_LOG_CORE_ERROR("Error log"));
  BOOST_CHECK_NO_THROW(TE_LOG_CORE_TRACE("Trace log"));
  BOOST_CHECK_NO_THROW(TE_LOG_CORE_FATAL("Fatal log"));
  BOOST_CHECK_NO_THROW(TE_LOG_CORE_DEBUG("Debug log"));
}

BOOST_AUTO_TEST_SUITE_END()

