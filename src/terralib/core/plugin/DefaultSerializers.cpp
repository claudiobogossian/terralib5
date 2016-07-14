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
  \file terralib/core/plugin/DefaultSerializers.cpp

  \brief A singleton that can be used to register plugin finders.

  \author Gilberto Ribeiro de Queiroz
  \author Matheus Cavassan Zaglia
 */

// TerraLib
#include "DefaultSerializers.h"
#include "Exception.h"

// Boost
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

te::core::PluginInfo
te::core::JSONPluginInfoSerializer(const std::string& file_name)
{
  boost::property_tree::ptree doc;

  boost::property_tree::json_parser::read_json(file_name, doc);

  PluginInfo plugin;

  plugin.name = doc.get<std::string>("name");
  plugin.display_name = doc.get<std::string>("display_name");
  plugin.description = doc.get<std::string>("description");
  plugin.version = doc.get<std::string>("version");
  plugin.release = doc.get<std::string>("release");
  plugin.engine = doc.get<std::string>("engine");
  plugin.license_description = doc.get<std::string>("license_description");
  plugin.license_URL = doc.get<std::string>("license_URL");
  plugin.site = doc.get<std::string>("site");
  //plugin.description = doc.get<std::string>("provider");
  //plugin.description = doc.get<std::string>("dependencies");

  for(const boost::property_tree::ptree::value_type& v: doc.get_child("resources"))
  {
    Resource r;

    r.first = v.first;
    r.second = v.second.get_value<std::string>();

    plugin.resources.push_back(r);
  }

  //plugin.description = doc.get<std::string>("parameters");
  //plugin.description = doc.get<std::string>("host_application");

  return plugin;
}
