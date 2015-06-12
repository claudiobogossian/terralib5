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

  \brief A list of examples for the TerraLib Serialization.
*/

#include "SerializationExamples.h"

// TerraLib
#include <terralib/common.h>
#include <terralib/plugin.h>
#include <terralib/plugin/PluginInfo.h>
#include <terralib/plugin/PluginManager.h>
#include <terralib/se/Style.h>
#include "../Config.h"

// STL
#include <cassert>
#include <exception>
#include <fstream>
#include <iostream>

int main(int /*argc*/, char** /*argv*/)
{
// Initializes Terralib support
  TerraLib::getInstance().initialize();


  te::plugin::PluginInfo pinfo;
  pinfo.m_name = "TERRALIB_XERCES";
  pinfo.m_category = "XML";
  pinfo.m_engine = TE_CPPPLUGINENGINE_CODE;
  //pinfo.m_folder = "."; //the default search directories will be used
  pinfo.m_resources.push_back(te::plugin::PluginInfo::Resource("SharedLibraryName", "terralib_mod_xerces"));

  te::plugin::PluginManager::getInstance().load(pinfo, true);




  try
  {
    /* OGC Symbology Encoding */

    std::string path = ""TERRALIB_DATA_DIR"/xml/style.xml";

    EncodeStyle(path); // Creates a Style and encodes it to XML format.

    te::se::Style* style = DecodeStyle(path); // Decodes the created XML file.

    /* XSD Schema */

    std::cout << "Decoding schema..." << std::endl;
    DecodeSchema(""TERRALIB_DATA_DIR"/xml/plugin_info.xsd");
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

// Finalizes Terralib support
  TerraLib::getInstance().finalize();

  return EXIT_SUCCESS;
}
