/*  Copyright (C) 2010-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/datatype/Module.cpp
   
  \brief This singleton defines the TerraLib Data Type module entry.
*/

// TerraLib
#include <terralib/common/Logger.h>
#include <terralib/common/TerraLib.h>
#include <terralib/common/Translator.h>
#include <terralib/common/OSSettingsDir.h>
#include <terralib/common/SystemApplicationSettings.h>
#include "Config.h"
#include "Module.h"
#include "CoreApplication.h"

//! Boost include files
#include <boost/filesystem.hpp>

const te::qt::af::Module& sm_module = te::qt::af::Module::getInstance();

namespace fs = boost::filesystem;
namespace b_prop = boost::property_tree;

void getDefaultConfigurations(std::vector< std::pair<std::string, std::string> >& configs)
{
  configs.push_back(std::pair<std::string, std::string>("Application.<xmlattr>.xmlns:xsd", "http://www.w3.org/2001/XMLSchema-instance"));
  configs.push_back(std::pair<std::string, std::string>("Application.<xmlattr>.xmlns", "http://www.terralib.org/schemas/af"));
  configs.push_back(std::pair<std::string, std::string>("Application.<xmlattr>.xsd:schemaLocation", "http://www.terralib.org/schemas/af ../myschemas/terralib/af/af.xsd"));
  configs.push_back(std::pair<std::string, std::string>("Application.<xmlattr>.version", "5.0.0"));
  configs.push_back(std::pair<std::string, std::string>("Application.<xmlattr>.release", "2011-01-01"));
  configs.push_back(std::pair<std::string, std::string>("Application.Organization", "INPE"));
  configs.push_back(std::pair<std::string, std::string>("Application.Name", "TerraLib"));
}

void configureSystem(const std::vector< std::pair<std::string, std::string> >& configs)
{
  std::vector< std::pair<std::string, std::string> >::const_iterator it;
  b_prop::ptree* p = &te::common::SystemApplicationSettings::getInstance().getAllSettings();

  for(it=configs.begin(); it != configs.end(); ++it)
    p->add(it->first, it->second);

  te::common::SystemApplicationSettings::getInstance().changed();
}

te::qt::af::Module::Module()
{
  TerraLib::Module m = { TE_QT_AF_MODULE_NAME,
                         te::qt::af::Module::initialize,
                         te::qt::af::Module::finalize
                       };

// initialize TerraLib singleton
  TerraLib::getInstance().add(m);

// it initializes the Translator support for the TerraLib Data Type support
  TE_ADD_TEXT_DOMAIN(TE_QT_AF_TEXT_DOMAIN, TE_QT_AF_TEXT_DOMAIN_DIR, "UTF-8");
}

te::qt::af::Module::~Module()
{
  TerraLib::getInstance().remove(TE_QT_AF_MODULE_NAME);
}

void te::qt::af::Module::initialize()
{
  TE_LOG_TRACE(TR_QT_AF("TerraLib Application Framework module initialized!"));

  std::string settingsPath = te::common::OSSettingsDir::getInstance().getSystemSettingsPath();
  const std::string* app_name = teApp::getInstance().getApplicationName();
  std::string app_dir = (app_name->empty()) ? "terralib" : *app_name;
 // std::string userSettingsPath = te::common::OSSettingsDir::getInstance().getUserSettingsPath();

  std::string sys_set = settingsPath + "/" + app_dir + "/config.xml";

  fs::path p(sys_set);

  if(fs::exists(p) && fs::is_regular_file(p))
  {
    te::common::SystemApplicationSettings::getInstance().load(p.generic_string());
  }
  else
  {
    if(!fs::exists(p.branch_path()))
      fs::create_directories(p.branch_path());

    te::common::SystemApplicationSettings::getInstance().load(p.generic_string());

    const std::vector< std::pair<std::string, std::string> >* af_configs = te::qt::af::teApp::getInstance().getApplicationInfo();

    if(af_configs->empty())
    {
      std::vector< std::pair<std::string, std::string> > configs;
      getDefaultConfigurations(configs);
      configureSystem(configs);
    }
    else
      configureSystem(*af_configs);

    te::common::SystemApplicationSettings::getInstance().update();
  }
}

void te::qt::af::Module::finalize()
{
  TE_LOG_TRACE(TR_QT_AF("TerraLib Application Framework module finalized!"));
}

