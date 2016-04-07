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
  \file terralib/unittest/core/lib/TsLibrary.cpp

  \brief A test suit for the TerraLib Core Lib Module.

  \author Frederico Augusto Bedê.
 */

// STL
#include <iostream>

// TerraLib
#include <terralib/core/lib/Library.h>
#include <terralib/core/lib/Exception.h>

// Boost
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE( lib_test_case ) 

BOOST_AUTO_TEST_CASE(test_constructor)
{
  te::core::Library* l1 = 0;
  
  /* Unloaded library. */
  /* -------------------- */
  // Shared library exist.
  BOOST_CHECK(l1 = new te::core::Library("functionsd.dll", true));
  delete l1;
  
  // Shared library do not exist.
  BOOST_CHECK(l1 = new te::core::Library("fred.dll", true));
  delete l1;

  // Name of the library is empty
  // -> This should throw an exception...
  BOOST_CHECK(l1 = new te::core::Library("", true));
  delete l1;

  /* Loaded library. */
  /* -------------------- */
  // Shared library exist.
  BOOST_CHECK(l1 = new te::core::Library("functionsd.dll"));
  delete l1;

  // Shared library does not exist.
  BOOST_CHECK_THROW(l1 = new te::core::Library("fred.dll"), te::core::LibraryLoadException);

  // Name of the library is empty
  BOOST_CHECK_THROW(l1 = new te::core::Library(""), te::core::LibraryLoadException);

  return;
}

BOOST_AUTO_TEST_CASE(test_destructor)
{
  te::core::Library* l = new te::core::Library("functionsd.dll", true);

  BOOST_CHECK_NO_THROW(delete l);

  l = new te::core::Library("functionsd.dll");

  BOOST_CHECK_NO_THROW(delete l);
}

BOOST_AUTO_TEST_CASE(test_load)
{
  /* Correct */
  te::core::Library l1("functionsd.dll", true);
  BOOST_CHECK_NO_THROW(l1.load());

  // Loading twice
  BOOST_CHECK_NO_THROW(l1.load());

  // Shared library do not exist.
  te::core::Library l2("fred.dll", true);
  BOOST_CHECK_THROW(l2.load(), te::core::LibraryLoadException);

  // Name of the library is empty
  te::core::Library l3("", true);
  BOOST_CHECK_THROW(l3.load(), te::core::LibraryLoadException);

  return;
}

BOOST_AUTO_TEST_CASE(test_unload)
{
  /* Correct */
  te::core::Library l1("functionsd.dll");
  BOOST_CHECK_NO_THROW(l1.unload());

  /* Existing library but not loaded yet */
  te::core::Library l2("functionsd.dll", true);
  BOOST_CHECK_NO_THROW(l1.unload());

  // Shared library do not exist.
  te::core::Library l3("fred.dll", true);
  BOOST_CHECK_THROW(l3.unload(), te::core::LibraryUnloadException);

  // Name of the library is empty
  te::core::Library l4("", true);
  BOOST_CHECK_THROW(l4.unload(), te::core::LibraryUnloadException);

  return;
}

BOOST_AUTO_TEST_CASE(test_isloaded)
{
  /* Correct */
  te::core::Library l1("functionsd.dll");
  BOOST_CHECK(l1.isLoaded());
  l1.unload();
  BOOST_CHECK(!l1.isLoaded());
  l1.load();
  BOOST_CHECK(l1.isLoaded());

  /* Existing library but not loaded yet */
  te::core::Library l2("functionsd.dll", true);
  BOOST_CHECK(!l2.isLoaded());

  // Shared library do not exist.
  te::core::Library l3("fred.dll", true);
  BOOST_CHECK(!l3.isLoaded());

  // Name of the library is empty
  te::core::Library l4("", true);
  BOOST_CHECK(!l4.isLoaded());

  return;
}

BOOST_AUTO_TEST_CASE(test_getFileName)
{
  /* Correct */
  std::string fileName("functionsd.dll");
  te::core::Library l1(fileName);
  BOOST_CHECK(l1.getFileName().compare(fileName) == 0);

  return;
}

BOOST_AUTO_TEST_CASE(test_getAddress)
{
  /* Correct */
  std::string fileName("functionsd.dll");
  te::core::Library l1(fileName);

  BOOST_CHECK(l1.getAddress("fatorial"));
  BOOST_CHECK_THROW(l1.getAddress("fatorializando"), te::core::LibrarySymbolNotFoundException);

  return;
}

BOOST_AUTO_TEST_CASE(test_getNativeName)
{
  /* Correct */
  std::string nname = te::core::Library::getNativeName("functions");
  BOOST_CHECK((nname.compare("functions.dll") == 0) || 
              (nname.compare("functionsd.dll") == 0));

  return;
}

BOOST_AUTO_TEST_CASE(test_addSearchDir)
{
  /* Correct */
  BOOST_CHECK_NO_THROW(te::core::Library::addSearchDir("C:/Qt/5.5/msvc2013_64/bin"));

  BOOST_CHECK_THROW(te::core::Library::addSearchDir(""), te::core::LibraryInvalidSearchPathException);

  BOOST_CHECK_THROW(te::core::Library::addSearchDir("X:/funcate"), te::core::LibraryInvalidSearchPathException);

  BOOST_CHECK_THROW(te::core::Library::addSearchDir("X:/funcate/kkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkk"), te::core::LibraryInvalidSearchPathException);

  /* Function */
  BOOST_CHECK_THROW(te::core::Library("Qt5Core.dll"), te::core::LibraryLoadException);

  te::core::Library::addSearchDir("C:/Qt/5.5/msvc2013_64/bin");
  BOOST_CHECK_NO_THROW(te::core::Library("Qt5Core.dll"));

  return;
}

BOOST_AUTO_TEST_CASE(test_resetSearchDir)
{
  BOOST_CHECK_NO_THROW(te::core::Library::resetSearchPath());

  te::core::Library::addSearchDir("C:/Qt/5.5/msvc2013_64/bin");

  te::core::Library l("Qt5Core.dll", true);

  BOOST_CHECK_NO_THROW(l.load());

  l.unload();

  te::core::Library::resetSearchPath();

  BOOST_CHECK_THROW(te::core::Library("Qt5Core.dll"), te::core::LibraryLoadException);

  return;
}

BOOST_AUTO_TEST_SUITE_END()