/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/af/ApplicationPlugins.cpp

  \brief A singleton for managing the application plugins.
*/

// TerraLib
//#include "../../common/OSSettingsDir.h"
#include "../../common/SystemApplicationSettings.h"
#include "../../common/Translator.h"
#include "ApplicationPlugins.h"
#include "Exception.h"
#include "Utils.h"

// Boost
#include <boost/filesystem.hpp>


void te::qt::af::ApplicationPlugins::load()
{
  m_settings.clear();

// retrieving the installed plugins.
  std::vector<std::string> plgFiles = GetPluginsFiles();
  std::vector<std::string> plgNames = GetPluginsNames(plgFiles);

// adding them to the internal ptree
  for(std::size_t i = 0; i < plgFiles.size(); i++)
  {
    boost::property_tree::ptree plg;
    plg.add("Name", plgNames[i]);
    plg.add("Path.<xmlattr>.xlink:href", plgFiles[i]);
    m_settings.add_child("Plugins.Plugin", plg);
  }
}

void te::qt::af::ApplicationPlugins::load(const std::string& fileName)
{
  ApplicationSettings::load(fileName);
}

const std::map<std::string, std::string>& te::qt::af::ApplicationPlugins::getPluginsFiles() const
{
  return m_plugins_files;
}

void te::qt::af::ApplicationPlugins::addPlugin(const std::string& pluginName, const std::string& pluginFileName)
{
  if(m_plugins_files.find(pluginName) == m_plugins_files.end())
    m_plugins_files[pluginName] = pluginFileName;
}

void te::qt::af::ApplicationPlugins::removePlugin(const std::string& pluginName)
{
  std::map<std::string, std::string>::iterator it = m_plugins_files.find(pluginName);

  if(it != m_plugins_files.end())
    m_plugins_files.erase(it);
}

const std::string& te::qt::af::ApplicationPlugins::getFileName() const
{
  return m_file;
}

void te::qt::af::ApplicationPlugins::addPlugins(const std::map<std::string, std::string>& plgs)
{
  std::map<std::string, std::string>::const_iterator it;

  for(it=plgs.begin(); it!=plgs.end(); ++it)
    addPlugin(it->first, it->second);
}

void te::qt::af::ApplicationPlugins::removePlugins(const std::vector<std::string>& plgs)
{
  std::vector<std::string>::const_iterator it;

  for(it=plgs.begin(); it!=plgs.end(); ++it)
    removePlugin(*it);
}

te::qt::af::ApplicationPlugins::ApplicationPlugins()
  : QObject()
{
}

te::qt::af::ApplicationPlugins::~ApplicationPlugins()
{
}


