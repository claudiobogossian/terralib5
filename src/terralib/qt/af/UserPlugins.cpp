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
  \file terralib/qt/af/UserPlugins.cpp

  \brief A singleton for managing the plugins enabled by a specific user.
*/

// Boost => don't change this include order, otherwise you may have compile problems!
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>

// TerraLib
//#include "../../common/OSSettingsDir.h"
#include "../../common/SystemApplicationSettings.h"
#include "../../common/Translator.h"
#include "../../common/UserApplicationSettings.h"
#include "../../plugin/PluginInfo.h"
#include "../../plugin/PluginManager.h"
#include "../../plugin/Utils.h"
#include "ApplicationPlugins.h"
#include "Exception.h"
#include "UserPlugins.h"

// STL
#include <memory>
#include <set>


void te::qt::af::UserPlugins::load()
{
// firs we need to acquire all the locks in order (otherwise we may dead-lock!): application plugins and then user plugins
  ApplicationPlugins::LockRead l1(ApplicationPlugins::getInstance());

  te::common::UserApplicationSettings::LockRead l2(te::common::UserApplicationSettings::getInstance());

// the list of installed plugins
  std::set<std::pair<std::string, std::string> > sPlugins;
  bool child_exists = false;

// get system plugins
  {
    boost::property_tree::ptree& p = ApplicationPlugins::getInstance().getAllSettings();

    child_exists = p.count("Plugins") > 0;

    if(child_exists && (!p.empty()))
      BOOST_FOREACH(boost::property_tree::ptree::value_type& v, p.get_child("Plugins"))
      {
        std::string first = v.first.data();

        if(first != "Plugin")
          continue;

        const std::string& pname = v.second.get<std::string>("Name");
        const std::string& pdir = v.second.get<std::string>("Path.<xmlattr>.xlink:href");

        sPlugins.insert(std::pair<std::string, std::string>(pname, pdir));
      }
  }

// get specific user plugins
  {
    boost::property_tree::ptree& p = te::common::UserApplicationSettings::getInstance().getAllSettings().get_child("UserSettings");

    child_exists = p.count("SpecificPlugins") > 0;

    if(child_exists && (!p.empty()))
      BOOST_FOREACH(boost::property_tree::ptree::value_type& v, p.get_child("SpecificPlugins"))
      {
        if(v.second.data().empty())
          continue;

        const std::string& pname = v.second.get<std::string>("Name");
        const std::string& pdir = v.second.get<std::string>("Path.<xmlattr>.xlink:href");

        sPlugins.insert(std::pair<std::string, std::string>(pname, pdir));
      }
  }

// the plugins enabled by the user
  std::set<std::string> uPlugins;

  {
    boost::property_tree::ptree& p = te::common::UserApplicationSettings::getInstance().getAllSettings().get_child("UserSettings");

    child_exists = p.count("EnabledPlugins") > 0;

    if(child_exists && (!p.empty()))
      BOOST_FOREACH(boost::property_tree::ptree::value_type& v, p.get_child("EnabledPlugins"))
      {
        if(v.second.data().empty())
          continue;

        uPlugins.insert(v.second.data());
      }
  }

// retrieve plugin information about each user plugin
  boost::ptr_vector<te::plugin::PluginInfo> plugins;
  std::set<std::pair<std::string, std::string> >::const_iterator it = sPlugins.begin();
  std::set<std::pair<std::string, std::string> >::const_iterator itend = sPlugins.end();
  std::set<std::string>::const_iterator ituend = uPlugins.end();

  while(it != itend)
  {
    std::set<std::string>::const_iterator itu = uPlugins.find(it->first);

    try
    {
      te::plugin::PluginInfo* pinfo = te::plugin::GetInstalledPlugin(it->second);

      if(itu != ituend)
        plugins.push_back(pinfo);
      else
        te::plugin::PluginManager::getInstance().add(pinfo);
    }
    catch(...)
    {
    }

    ++it;
  }

// load and start each plugin
  te::plugin::PluginManager::getInstance().load(plugins);

  return;
}

te::qt::af::UserPlugins::UserPlugins()
{
}

te::qt::af::UserPlugins::~UserPlugins()
{
}

