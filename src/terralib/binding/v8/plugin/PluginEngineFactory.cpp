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
  \file PluginEngineFactory.cpp

  \brief This is the concrete factory for JavaScript plugin engines.
 */

// TerraLib
#include "PluginEngine.h"
#include "PluginEngineFactory.h"

te::v8::plugin::PluginEngineFactory* te::v8::plugin::PluginEngineFactory::sm_factory(0);

te::plugin::AbstractPluginEngine* te::v8::plugin::PluginEngineFactory::build()
{
  return new PluginEngine;
}

te::v8::plugin::PluginEngineFactory::PluginEngineFactory()
  : te::plugin::PluginEngineFactory(TE_V8_PLUGIN_ENGINE_CODE)
{
}

te::v8::plugin::PluginEngineFactory::~PluginEngineFactory()
{
}

void te::v8::plugin::PluginEngineFactory::initialize()
{
  finalize();
  sm_factory = new PluginEngineFactory;
}

void te::v8::plugin::PluginEngineFactory::finalize()
{
  delete sm_factory;
  sm_factory = 0;
}
