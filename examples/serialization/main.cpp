/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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

  \brief A list of examples for the TerraLib Serialization.
*/

#include "SerializationExamples.h"

// TerraLib
#include <terralib/common.h>
#include <terralib/plugin.h>
#include <terralib/se/Style.h>

// STL
#include <cassert>
#include <exception>
#include <fstream>
#include <iostream>

void ShowResult(const std::string& data)
{
  std::cout << "Result:" << std::endl;
  std::cout << data << std::endl;
}

void SaveResult(const std::string& data, const std::string& path)
{
  std::ofstream ofs(path);
  ofs << data;
}

int main(int /*argc*/, char** /*argv*/)
{
// Initializes Terralib support
  TerraLib::getInstance().initialize();

  try
  {
    /* OGC Symbology Encoding */

    std::string encodedStyle = EncodeStyle(); // Creates a Style and encodes it to XML format.

    ShowResult(encodedStyle); // Shows the serializaton result.
    
    SaveResult(encodedStyle, "D:/style.xml"); // Saves the enconded style to XML file.

    te::se::Style* style = DecodeStyle("D:/style.xml"); // Decodes the created XML file.

    std::string backData = EncodeStyle(style); // Encodes again.
    
    // Comparing the results...
    encodedStyle == backData ? std::cout << "Great job!" << std::endl : std::cout << "You are doing it wrong!" << std::endl;
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occuried: " << e.what() << std::endl;

    std::cout << "Press Enter to exit..." << std::endl;
    std::cin.get();

    return EXIT_FAILURE;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occuried!" << std::endl;

    std::cout << "Press Enter to exit..." << std::endl;
    std::cin.get();

    return EXIT_FAILURE;
  }

// Finalizes Terralib support
  TerraLib::getInstance().finalize();

  std::cout << "Press Enter to exit..." << std::endl;
  std::cin.get();

  return EXIT_SUCCESS;
}
