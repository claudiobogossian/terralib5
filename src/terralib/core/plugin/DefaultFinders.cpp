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
  \file terralib/core/plugin/PluginFinderManager.cpp

  \brief A singleton that can be used to register plugin finders.

  \author Gilberto Ribeiro de Queiroz
  \author Matheus Cavassan Zaglia
 */

// TerraLib
#include "DefaultFinders.h"
#include "../utils/Platform.h"
#include "Exception.h"
#include "DefaultSerializers.h"


// Boost
#include <boost/algorithm/string/predicate.hpp>
#include <boost/filesystem.hpp>

static std::vector<std::string> GetDefaultLookupDirs();

std::vector<te::core::PluginInfo>
te::core::DefaultPluginFinder()
{
// Retrieve the list of default directories to look up.
  std::vector<std::string> lookup_dirs = GetDefaultLookupDirs();

  std::vector<PluginInfo> found_plugins;

  for(const std::string& plugins_dir : lookup_dirs)
  {
    if(plugins_dir.empty() || !boost::filesystem::is_directory(plugins_dir))
      continue;

    boost::filesystem::path p(plugins_dir);

    for(boost::filesystem::directory_iterator it(p), it_end; it != it_end; ++it)
    {
      boost::filesystem::path pfile = (*it);

      if(!boost::filesystem::is_regular_file(pfile))
        continue;

      std::string file_name = pfile.string();

      if(boost::algorithm::ends_with(file_name, ".teplg.json"))
      {
        PluginInfo pinfo = JSONPluginInfoSerializer(file_name);

        found_plugins.push_back(pinfo);
      }
    }
  }
  
  return found_plugins;
}

static std::vector<std::string>
GetDefaultLookupDirs()
{
  std::vector<std::string> plugin_directories;

// Add application current directory as a search path
  plugin_directories.push_back(boost::filesystem::system_complete( "." ).string());

// Let's check if there is a directory called "share/terralib/plugins" in the application path
  std::string plg_dir = te::core::FindInTerraLibPath("share/terralib/plugins");

  if(!plg_dir.empty())
    plugin_directories.push_back(plg_dir);

  return plugin_directories;
}
