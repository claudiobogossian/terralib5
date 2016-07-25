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
#include <terralib/core/Exception.h>

// Boost
#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>

BOOST_AUTO_TEST_SUITE(logger_test_case)

BOOST_AUTO_TEST_CASE(default_logger_test)
{
  BOOST_CHECK(!te::core::Logger::instance().exists(TERRALIB_DEFAULT_LOGGER));
  BOOST_CHECK_NO_THROW(TE_INIT_DEFAULT_LOGGER("log/unit_test_terralib.log"));
  BOOST_CHECK(te::core::Logger::instance().exists(TERRALIB_DEFAULT_LOGGER));
  BOOST_CHECK_THROW(TE_INIT_DEFAULT_LOGGER("log/unit_test_terralib.log"), te::InvalidArgumentException);
  BOOST_CHECK_NO_THROW(TE_LOG_WARN("Warning log"));
  BOOST_CHECK_NO_THROW(TE_LOG_INFO("Info log"));
  BOOST_CHECK_NO_THROW(TE_LOG_ERROR("Error log"));
  BOOST_CHECK_NO_THROW(TE_LOG_TRACE("Trace log"));
  BOOST_CHECK_NO_THROW(TE_LOG_FATAL("Fatal log"));
  BOOST_CHECK_NO_THROW(TE_LOG_DEBUG("Debug log"));
  BOOST_CHECK(boost::filesystem::file_size("log/unit_test_terralib.log") > 0);

  return;
}

BOOST_AUTO_TEST_CASE(configuration_file_logger_test)
{
  BOOST_CHECK_THROW(TE_ADD_LOGGER_FROM_FILE("mylogger.ini"), std::exception);
  BOOST_CHECK_NO_THROW(TE_ADD_LOGGER_FROM_FILE(
                         te::core::FindInTerraLibPath("share/terralib/config/te-log-unittest.ini")));
  BOOST_CHECK_NO_THROW(TE_CORE_LOG_WARN("unittest","Warning log"));
  BOOST_CHECK_NO_THROW(TE_CORE_LOG_INFO("unittest","Info log"));
  BOOST_CHECK_NO_THROW(TE_CORE_LOG_ERROR("unittest","Error log"));
  BOOST_CHECK_NO_THROW(TE_CORE_LOG_TRACE("unittest","Trace log"));
  BOOST_CHECK_NO_THROW(TE_CORE_LOG_FATAL("unittest","Fatal log"));
  BOOST_CHECK_NO_THROW(TE_CORE_LOG_DEBUG("unittest","Debug log"));
  BOOST_CHECK(boost::filesystem::file_size(te::core::FindInTerraLibPath("log/terralib_unittest_core.log")) > 0);

  return;
}

BOOST_AUTO_TEST_SUITE_END()

