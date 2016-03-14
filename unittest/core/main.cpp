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
  \file terralib/unittest/core/main.cpp

  \brief Main file of test suit for the Core Runtime Library.

  \author Vinicius campanha
 */

// STL
#include <cstdlib>

// Boost
#define BOOST_TEST_NO_MAIN
#include <boost/test/unit_test.hpp>

bool init_unit_test()
{
  return true;
}

int main(int argc, char *argv[])
{
  boost::unit_test::unit_test_main(init_unit_test, argc, argv);

  return EXIT_SUCCESS;
}
