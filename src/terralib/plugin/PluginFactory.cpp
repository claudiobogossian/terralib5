/*  Copyright (C) 2008-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file PluginFactory.cpp

  \brief This is the abstract factory for Plugins.
 */

// TerraLib
#include "PluginFactory.h"
/*
te::plugin::Plugin* te::plugin::PluginFactory::make(const std::string& fileName)
{
  size_t pos = fileName.rfind(".");

  if(pos == std::string::npos)
    return 0;

  std::string factoryKey = fileName.substr(pos + 1);


  return te::common::AbstractFactory<Plugin, std::string>::make(factoryKey);
}
*/
te::plugin::PluginFactory::~PluginFactory()
{
}

te::plugin::PluginFactory::PluginFactory(const std::string& factoryKey)
  : te::common::ParameterizedAbstractFactory<Plugin, std::string, PluginInfo>(factoryKey)
{
}


