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
  \file terralib/unittest/core/utils/TsPlatform.cpp

  \brief A test suite for the TerraLib Core Utils Module.

  \author Carolina Galvão dos Santos
 */

// STL
#include <string>

// TerraLib
#include <terralib/core/utils/Platform.h>

// Boost
#include <boost/test/unit_test.hpp>
#include <boost/algorithm/string/predicate.hpp>

BOOST_AUTO_TEST_SUITE( platform_tests )

BOOST_AUTO_TEST_CASE( findpath_correct_tests )
{

  /* Empty path */
  BOOST_CHECK(te::core::FindInTerraLibPath("") != "");

  /* Search in the same path */
  BOOST_CHECK(boost::algorithm::ends_with(te::core::FindInTerraLibPath("/terralib_unittest_core"),"/terralib_unittest_core"));

  /* Search in the previous path */
  BOOST_CHECK(boost::algorithm::ends_with(te::core::FindInTerraLibPath("/share"),"/share"));

  /* Search in codebase path */
  BOOST_CHECK(boost::algorithm::ends_with(te::core::FindInTerraLibPath("/src"),"/src"));

  /* Search without / */
  BOOST_CHECK(boost::algorithm::ends_with(te::core::FindInTerraLibPath("bin"),"/bin"));

  /* Search a file path */
  BOOST_CHECK(boost::algorithm::ends_with(te::core::FindInTerraLibPath("src/terralib/core/utils/Platform.cpp"),"src/terralib/core/utils/Platform.cpp"));

  return;
}

BOOST_AUTO_TEST_CASE( findpath_wrong_tests )
{

  /* Search path that doesn't exist */
  BOOST_CHECK(boost::algorithm::ends_with(te::core::FindInTerraLibPath("/abacate"),"/abacate") == false);

  /* Search an incorrect path*/
  BOOST_CHECK(boost::algorithm::ends_with(te::core::FindInTerraLibPath("/src/core/utils"),"/src/core/utils") == false);

  /* First directory is not in the search directories */
  BOOST_CHECK(boost::algorithm::ends_with(te::core::FindInTerraLibPath("/core/encoding"),"/core/encoding") == false);

  return;
}

BOOST_AUTO_TEST_SUITE_END()
