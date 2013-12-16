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

  \brief A list of examples for the TerraLib Xerces Module.
 */

// Example
#include "ReaderExample.h"

// TerraLib
#include <terralib/common/TerraLib.h>
#include <terralib/plugin.h>

// STL
#include <iostream>

void LoadXerces()
{
  try
  {
    te::plugin::PluginInfo pinfo;
    pinfo.m_name = "TERRALIB_XERCES";
    pinfo.m_category = "XML";
    pinfo.m_engine = TE_CPPPLUGINENGINE_CODE;
    pinfo.m_folder = TE_PLUGINS_PATH;
    pinfo.m_resources.push_back(te::plugin::PluginInfo::Resource("SharedLibraryName", "terralib_xerces"));

    te::plugin::PluginManager::getInstance().load(pinfo, true);
  }
  catch(...)
  {
    std::cout << std::endl << "Failed to load XERCES plugin: unknow exception!" << std::endl;
  }
}

int main(int /*argc*/, char** /*argv*/)
{
  try
  {
    TerraLib::getInstance().initialize();
    ReadXML(""TE_DATA_EXAMPLE_DIR"/data/xml/xerces/note.xml", true, false);
    ReadXML(""TE_DATA_EXAMPLE_DIR"/data/xml/xerces/note.xml", false, false);
    ReadXML(""TE_DATA_EXAMPLE_DIR"/data/xml/xerces/note.xml", false, true);
    //ReadXML(""TE_DATA_EXAMPLE_DIR"/data/xml/xerces/test.xml", false, true); //not found 
    //ReadXML(""TE_DATA_EXAMPLE_DIR"/data/xml/xerces/test_contracted.xml", false, true); //not found

    TerraLib::getInstance().finalize();
  }
  catch(const std::exception& e)
  {
    std::cout << std::endl << "An exception has occurried: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }
  catch(...)
  {
    std::cout << std::endl << "An unexpected exception has occurried!" << std::endl;
    return EXIT_FAILURE;
  }

  std::cout << "Press Enter to exit..." << std::endl;
  std::cin.get();

  return EXIT_SUCCESS;
}
