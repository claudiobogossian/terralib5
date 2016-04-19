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
\file terralib/examples/core/lib/main.cpp

\brief Examples for the TerraLib Library Module

\author Frederico Augusto Bedê
*/

// TerraLib
#include <terralib/Defines.h>
#include <terralib/core/lib/Library.h>
#include <terralib/core/lib/Exception.h>

// Boost
#include <boost/filesystem.hpp>

// STL
#include <iostream>
#include <string>

std::string GetExampleFolder()
{
#if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
  std::string path = boost::filesystem::complete(boost::filesystem::path("../example")).string();

#ifdef NDEBUG
  return path + "/Release";
#else
  return path + "/Debug";
#endif
#elif TE_PLATFORM == TE_PLATFORMCODE_LINUX || TE_PLATFORM == TE_PLATFORMCODE_APPLE
  return boost::filesystem::absolute("example").string();
#else
#error "Platform not supported yet! Please contact terralib-team@dpi.inpe.br"
#endif
}


int main(int argc, char *argv[])
{
  /* Loading library using full path. */
  try
  {
    /* Get the file name. */
    std::string lName = te::core::Library::getNativeName("terralib_example_core_lib_function");
 
#if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
    te::core::Library::addSearchDir(GetExampleFolder());
#elif TE_PLATFORM == TE_PLATFORMCODE_LINUX || TE_PLATFORM == TE_PLATFORMCODE_APPLE
    std::cout <<GetExampleFolder();
    lName = GetExampleFolder() + "/" + lName;
#endif


    /* The library is located at the same directory as the executable. */
    te::core::Library l1(lName);

    /* Testing if it was loaded. */
    if(l1.isLoaded())
      std::cout << std::endl << "Shared library " << l1.getFileName() << " loaded!" << std::endl;

    /* Getting a pointer to a function in the library. */
    void* t = l1.getAddress("fatorial");

    /* Testing the pointer. */
    if(t != nullptr)
      std::cout << "Function fatorial found!" << std::endl;
    
    typedef int (*fatorial_fptr)(int);
    
    fatorial_fptr fat = reinterpret_cast<fatorial_fptr>(t);
    
    std::cout << "Fatorial 5: " << fat(5) << std::endl;

    /* Unloading library. */
    l1.unload();

    std::cout << "Library unloaded!" << std::endl << std::endl;
  }
  catch(const te::core::LibraryLoadException& e)
  {
    if(const std::string* d = boost::get_error_info<te::ErrorDescription>(e))
      std::cout << std::endl << "Fail to load library: " << *d;
  }
  catch(const te::core::LibraryUnloadException& e)
  {
    if(const std::string* d = boost::get_error_info<te::ErrorDescription>(e))
      std::cout << std::endl << "Fail to unload library: " << *d;
  }
  catch(const te::core::LibrarySymbolNotFoundException& e)
  {
    if(const std::string* d = boost::get_error_info<te::ErrorDescription>(e))
      std::cout << std::endl << "Fail to load symbol: " << *d;
  }
  catch(const te::Exception& e)
  {
    if(const std::string* d = boost::get_error_info<te::ErrorDescription>(e))
      std::cout << std::endl << "Unknown error: " << *d;
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "Unknown error: " << e.what();
  }
  catch(...)
  {
    std::cout << std::endl << "Unknown error!";
  }

  std::cin.get();
}
