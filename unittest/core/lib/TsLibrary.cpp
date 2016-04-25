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

// TerraLib
#include <terralib/Defines.h>
#include <terralib/core/lib/Library.h>
#include <terralib/core/lib/Exception.h>

// Boost
#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>

// STL
#include <iostream>

std::string GetExampleFolder()
{
#if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
  std::string path = boost::filesystem::complete(boost::filesystem::path("../example")).string();

#ifdef NDEBUG
  return path + "/Release/";
#else
  return path + "/Debug/";
#endif
#elif TE_PLATFORM == TE_PLATFORMCODE_LINUX || TE_PLATFORM == TE_PLATFORMCODE_APPLE
  return boost::filesystem::absolute("example").string() + "/";
#else
#error "Platform not supported yet! Please contact terralib-team@dpi.inpe.br"
#endif
}

std::string GetNativeName(const bool& dir=true)
{
  std::string lName = te::core::Library::getNativeName("terralib_unittest_core_lib_function");

  if(dir == true)
    return GetExampleFolder() + lName;

  return lName;
}

#if TE_PLATFORM == TE_PLATFORMCODE_LINUX || TE_PLATFORM == TE_PLATFORMCODE_APPLE
std::string GetExePath()
{
  char result[ 255 ];
  ssize_t count = readlink( "/proc/self/exe", result, 255 );
  return std::string( result, (count > 0) ? count : 0 );
}
#endif

BOOST_AUTO_TEST_SUITE( lib_test_case ) 

BOOST_AUTO_TEST_CASE(test_constructor)
{
  te::core::Library* l1 = 0;
  std::string lName = GetNativeName();

  std::cout <<lName;
  
  /* Unloaded library. */
  /* -------------------- */
  // Shared library exist.
  BOOST_CHECK(l1 = new te::core::Library(lName, true));
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
  BOOST_CHECK(l1 = new te::core::Library(lName));
  delete l1;

  // Shared library does not exist.
  BOOST_CHECK_THROW(l1 = new te::core::Library("fred.dll"), te::core::LibraryLoadException);

  // Name of the library is empty
  BOOST_CHECK_THROW(l1 = new te::core::Library(""), te::core::LibraryLoadException);

  return;
}

BOOST_AUTO_TEST_CASE(test_destructor)
{
  std::string lName = GetNativeName();

  /* Testing destructor on a library not loaded. */
  te::core::Library* l = new te::core::Library(lName, true);

  BOOST_CHECK_NO_THROW(delete l);

  /* Testing destructor on a loaded library. */
  l = new te::core::Library(lName);

  BOOST_CHECK_NO_THROW(delete l); 

  return;
}

BOOST_AUTO_TEST_CASE(test_load)
{
  std::string lName = GetNativeName();

  /* Correct */
  te::core::Library l1(lName, true);
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
  std::string lName = GetNativeName();

  /* Correct */
  te::core::Library l1(lName);
  BOOST_CHECK_NO_THROW(l1.unload());

  /* Existing library but not loaded yet */
  te::core::Library l2(lName, true);
  BOOST_CHECK_NO_THROW(l1.unload());

  // Shared library does not exist.
  te::core::Library l3("fred.dll", true);
  BOOST_CHECK_THROW(l3.unload(), te::core::LibraryUnloadException);

  // Name of the library is empty
  te::core::Library l4("", true);
  BOOST_CHECK_THROW(l4.unload(), te::core::LibraryUnloadException);

  return;
}

BOOST_AUTO_TEST_CASE(test_isloaded)
{
  std::string lName = GetNativeName();

  /* Correct */
  te::core::Library l1(lName);
  BOOST_CHECK(l1.isLoaded());
  l1.unload();
  BOOST_CHECK(!l1.isLoaded());
  l1.load();
  BOOST_CHECK(l1.isLoaded());

  /* Existing library but not loaded yet */
  te::core::Library l2(lName, true);
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
  std::string lName = GetNativeName();

  /* Checking fileName */
  std::string fileName(lName);
  te::core::Library l1(fileName);
  BOOST_CHECK(l1.getFileName().compare(fileName) == 0);

  return;
}

BOOST_AUTO_TEST_CASE(test_getAddress)
{
  std::string lName = GetNativeName();

  /* Correct */
  te::core::Library l1(lName);

  /* Testing get a function that exists. */
  BOOST_CHECK(l1.getAddress("fatorial"));
  
  /* Testing get a function that does not exists. */
  BOOST_CHECK_THROW(l1.getAddress("fatorializando"), te::core::LibrarySymbolNotFoundException);

  return;
}

BOOST_AUTO_TEST_CASE(test_getNativeName)
{
  /* Correct */
  std::string name("terralib_unittest_core_lib_function");
  std::string nname = te::core::Library::getNativeName(name);
  std::string prefix,
    suffix;

#if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
#ifdef NDEBUG
  suffix = ".dll";
#else
  suffix = "d.dll";
#endif
#elif TE_PLATFORM == TE_PLATFORMCODE_LINUX 
  prefix = "lib";
  suffix = ".so";
#elif TE_PLATFORM == TE_PLATFORMCODE_APPLE
  prefix = "lib"
  suffix = ".dylib";
#else
#error "Platform not supported yet! Please contact terralib-team@dpi.inpe.br"
#endif

  std::string trueName = prefix + name + suffix;

  BOOST_CHECK(nname.compare(trueName) == 0);

  return;
}

#if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
BOOST_AUTO_TEST_CASE(test_addSearchDir)
{
  std::string lName = te::core::Library::getNativeName("terralib_unittest_core_lib_function");

  /* Correct */
  BOOST_CHECK_NO_THROW(te::core::Library::addSearchDir(GetExampleFolder()));

  /* Empty path. */
  BOOST_CHECK_THROW(te::core::Library::addSearchDir(""), te::core::LibraryInvalidSearchPathException);

  /* Path that does not exist. */
  BOOST_CHECK_THROW(te::core::Library::addSearchDir("X:/funcate"), te::core::LibraryInvalidSearchPathException);

  /* Path with more characters than maximum size. */
  BOOST_CHECK_THROW(te::core::Library::addSearchDir("X:/funcate/kkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkk"), te::core::LibraryInvalidSearchPathException);

  /* Trying to load a library that can not be found by the operational system. */
  BOOST_CHECK_THROW(te::core::Library(lName.c_str()), te::core::LibraryLoadException);  

  te::core::Library::addSearchDir(GetExampleFolder());

  /* Trying to load a library that can be found by the operational system. */

  BOOST_CHECK_NO_THROW(te::core::Library(lName.c_str()));

  return;
}

BOOST_AUTO_TEST_CASE(test_resetSearchDir)
{
  std::string lName = te::core::Library::getNativeName("terralib_unittest_core_lib_function");

  /* Testing reset on an unchanged path. */
  BOOST_CHECK_NO_THROW(te::core::Library::resetSearchPath());

  /* Testing load a shared library adding a search path. */
  te::core::Library::addSearchDir(GetExampleFolder());
  te::core::Library l(lName, true);
  BOOST_CHECK_NO_THROW(l.load());

  l.unload();

  /* Testing load library after reset search path. */
  te::core::Library::resetSearchPath();
  BOOST_CHECK_THROW(te::core::Library(lName.c_str()), te::core::LibraryLoadException);

  return;
}
#endif

BOOST_AUTO_TEST_SUITE_END()
