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
  \file terralib/plugin/Utils.cpp

  \brief Utility functions for dealing with plugins.
*/

// TerraLib
#include "../common/Translator.h"
#include "../xml/Reader.h"
#include "../xml/ReaderFactory.h"
#include "AbstractPlugin.h"
#include "Exception.h"
#include "PluginInfo.h"
#include "PluginManager.h"
#include "Utils.h"

// Boost
#include <boost/filesystem.hpp>
#include <boost/format.hpp>

void te::plugin::UnloadAllPluginsFromEngine(const std::string& engine)
{
  std::vector<std::string> plugins = PluginManager::getInstance().getPlugins();

  std::vector<std::string>::reverse_iterator it = plugins.rbegin();
  std::vector<std::string>::reverse_iterator itend = plugins.rend();

  while(it != itend)
  {
// if plugin wasn't find... maybe because we have propagate an unload...
    if(!PluginManager::getInstance().isLoaded(*it))
    {
      ++it;
      continue;
    }

// if we have found it... let's unload it!
    const PluginInfo& plugin = PluginManager::getInstance().getPlugin(*it);

    if(plugin.m_engine == engine)
      Unload(*it);

    ++it;
  }
}

void te::plugin::Unload(const std::string& plugin)
{
// check plugin dependency
  std::vector<std::string> pdependents = PluginManager::getInstance().getDependents(plugin);

  for(std::size_t i = 0; i < pdependents.size(); ++i)
    Unload(pdependents[i]);

  if(!PluginManager::getInstance().isLoaded(plugin))
    return;

  PluginManager::getInstance().unload(plugin);
}

te::plugin::PluginInfo* te::plugin::GetInstalledPlugin(const std::string& pluginFilePath)
{
  boost::filesystem::path pluginFileName(pluginFilePath);

// check if it was provided a plugin file name or just its dir
 /* if(boost::filesystem::is_directory(pluginFileName))
    pluginFileName /= TE_DEFAULT_PLUGIN_FILE_NAME;*/

// check
  if(!boost::filesystem::is_regular_file(pluginFileName))
    throw Exception((boost::format(TE_TR("The informed plugin file is not valid: %1%.")) % pluginFileName).str());

  std::auto_ptr<te::xml::Reader> xmlReader(te::xml::ReaderFactory::make());

  xmlReader->read(pluginFileName.string());

  if(!xmlReader->next())
    throw Exception(TE_TR("Could not read plugin information!"));

  if(xmlReader->getNodeType() != te::xml::START_ELEMENT)
    throw Exception(TE_TR("The document has problems!"));

  if(xmlReader->getElementLocalName() != "PluginInfo")
    throw Exception(TE_TR("The first tag in the document is not 'PluginInfo'!"));

  std::auto_ptr<PluginInfo> pInfo(new PluginInfo);
  *pInfo << *xmlReader;

  return pInfo.release();
}

