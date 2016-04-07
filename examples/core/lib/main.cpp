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

// STL
#include <iostream>

std::string GetExampleFolder()
{
#if TE_PLATFORM == TE_PLATFORMCODE_MSWINDOWS
#ifdef NDEBUG
  return "../example/Release/";
#else
  return "../example/Debug/";
#endif
#elif TE_PLATFORM == TE_PLATFORMCODE_LINUX || TE_PLATFORM == TE_PLATFORMCODE_APPLE
  return "example/bin/";
#else
#error "Platform not supported yet! Please contact terralib-team@dpi.inpe.br"
#endif
}

int main(int argc, char *argv[])
{
  /* Loading library using full path. */
  {
    try
    {
      /* Get the file name. */
      std::string lName = te::core::Library::getNativeName("functions");

      /* Adds a path to find shared libraries. */
      te::core::Library::addSearchDir(GetExampleFolder());

      /* The library is located at the same directory as the executable. */
      te::core::Library l1(lName);

      /* Testing if it was loaded. */
      if(l1.isLoaded())
        std::cout <<"Shared library " <<l1.getFileName() <<" loaded!" <<std::endl;

      /* Getting a pointer to a function in the library. */
      void* t = l1.getAddress("fatorial");

      /* Testing the pointer. */
      if(t != nullptr)
        std::cout << std::endl << "Function fatorial found!" << std::endl;

      /* Unloading library. */
      l1.unload();
      std::cout << std::endl << "Library unloaded!";
    }
    catch(te::core::LibraryLoadException& e)
    {
      std::cout << std::endl << "Fail to load library: " << e.what();
    }
    catch(te::core::LibraryUnloadException& e)
    {
      std::cout << std::endl << "Fail to unload library: " << e.what();
    }
    catch(te::core::LibrarySymbolNotFoundException& e)
    {
      std::cout << std::endl << "Fail to load symbol: " << e.what();
    }
  }

  std::cin.get();
}