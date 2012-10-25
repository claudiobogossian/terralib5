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
  \file terralib/plugin/DefaultFinder.cpp

  \brief A plugin finder that search for plugins in some special directories defined by compile time macros.
*/

// TerraLib
#include "../common/Translator.h"
#include "DefaultFinder.h"
#include "PluginInfo.h"
#include "Utils.h"

// Boost
#include <boost/filesystem.hpp>
#include <boost/format.hpp>

// STL
#include <cassert>
#include <cstdlib>

te::plugin::DefaultFinder::DefaultFinder()
{
}

te::plugin::DefaultFinder::~DefaultFinder()
{
}

std::string te::plugin::DefaultFinder::getDefaultDir() const
{
// let's check if there is a directory called TE_DEFAULT_PLUGINS_DIR in the current application dir
  if(boost::filesystem::is_directory(TE_DEFAULT_PLUGINS_DIR))
    return boost::filesystem::system_complete(TE_DEFAULT_PLUGINS_DIR).string();

// if the default dir is not available in the current dir let's try an environment variable defined as TERRALIB_DIR_ENVIRONMENT_VARIABLE
  char* e = getenv(TERRALIB_DIR_ENVIRONMENT_VARIABLE);

  if(e != 0)
  {
    boost::filesystem::path p(e);
    p /= TE_DEFAULT_PLUGINS_DIR;

    if(boost::filesystem::is_directory(p))
      return boost::filesystem::system_complete(p).string();
  }

// Humm: default plugin dir not found!
  return "";
}

void te::plugin::DefaultFinder::addPluginsDir(const std::string& path)
{
  if(!boost::filesystem::is_directory(path))
    throw Exception((boost::format(TR_PLUGIN("Default plugin directory is invalid: %1%.")) % path).str());

  boost::filesystem::path p(boost::filesystem::system_complete(path));

  std::string s(p.string());

  if(std::find(m_pluginsDir.begin(), m_pluginsDir.end(), s) != m_pluginsDir.end())
    return; // dir already in the path

  m_pluginsDir.push_back(s);
}

const std::vector<std::string>& te::plugin::DefaultFinder::getPluginsDir() const
{
  return m_pluginsDir;
}

void te::plugin::DefaultFinder::getPlugins(boost::ptr_vector<PluginInfo>& plugins)
{
// is there a base dir for looking for plugins?
  std::size_t ndirs = m_pluginsDir.size();

  if(ndirs == 0)
  {
    std::string basedir = getDefaultDir();

    if(basedir.empty())
      return;

    m_pluginsDir.push_back(basedir);

    ndirs = 1;
  }

// let's look in each plugins base dir
  for(std::size_t i = 0; i < ndirs; ++i)
  {
    if(!boost::filesystem::is_directory(m_pluginsDir[i]))
      throw Exception((boost::format(TR_PLUGIN("The base plugin directory is invalid: %1%.")) % m_pluginsDir[i]).str());

    boost::filesystem::path path(m_pluginsDir[i]);

    for(boost::filesystem::directory_iterator it(path), itEnd; it != itEnd; ++it)
    {
// check just in direct sub-dirs, don't go recursively for ever!
      if(boost::filesystem::is_directory(it->status()))
      {
        boost::filesystem::path foundPlugin = (*it);
        
        foundPlugin /= TE_DEFAULT_PLUGIN_FILE_NAME;

        if(boost::filesystem::is_regular_file(foundPlugin))
        {
// try to read the plugin XML configuration file and add info to the output vector
          plugins.push_back(GetInstalledPlugin(foundPlugin.string()));
        }
      }
    }
  }
}
