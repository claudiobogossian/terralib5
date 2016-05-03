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
  \file Module.cpp
   
  \brief This singleton defines the TerraLib Qt Widgets module entry.
*/

// TerraLib
#include "../common/Logger.h"
#include "../common/TerraLib.h"
#include "../core/translator/Translator.h"
#include "Config.h"
#include "Module.h"

const te::addressgeocoding::Module& sm_module = te::addressgeocoding::Module::getInstance();


te::addressgeocoding::Module::Module()
{
  TerraLib::Module m = { TE_ADDRESSGEOCODING_MODULE_NAME,
                         te::addressgeocoding::Module::initialize,
                         te::addressgeocoding::Module::finalize
                       };

// initialize TerraLib singleton
  TerraLib::getInstance().add(m);
}

te::addressgeocoding::Module::~Module()
{
  TerraLib::getInstance().remove(TE_ADDRESSGEOCODING_MODULE_NAME);
}

void te::addressgeocoding::Module::initialize()
{
  TE_LOG_TRACE(TE_TR("TerraLib Address Geocoding initialized!"));
}

void te::addressgeocoding::Module::finalize()
{

  TE_LOG_TRACE(TE_TR("TerraLib Address Geocoding finalized!"));
}

