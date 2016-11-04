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
  \file main.cpp

  \brief A list of examples for the TerraLib Xerces Module.
 */

// Example
#include "ReaderExample.h"

// TerraLib
#include "../Config.h"
#include <terralib/common/TerraLib.h>


// STL
#include <iostream>

int main(int /*argc*/, char** /*argv*/)
{
  try
  {
    TerraLib::getInstance().initialize();
    ReadXML("/home/vinicius/MyDevel/terralib5-git/examples/xerces/note.xml", true, false);
    ReadXML("/home/vinicius/MyDevel/terralib5-git/examples/xerces/note.xml", false, false);
    ReadXML("/home/vinicius/MyDevel/terralib5-git/examples/xerces/note.xml", false, true);
    //ReadXML(""TE_DATA_EXAMPLE_DIR"/xml/xerces/test.xml", false, true); //not found
    //ReadXML(""TE_DATA_EXAMPLE_DIR"/xml/xerces/test_contracted.xml", false, true); //not found

    TerraLib::getInstance().finalize();
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occurred: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occurred!" << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
