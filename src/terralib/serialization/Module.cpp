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
  \file Module.cpp
   
  \brief This singleton defines the TerraLib Serialization module entry.
*/

// TerraLib
#include "../common/Logger.h"
#include "../common/TerraLib.h"
#include "../common/Translator.h"
#include "Config.h"
#include "Module.h"

const te::serialize::Module& sm_module = te::serialize::Module::getInstance();

te::serialize::Module::Module()
{
  TerraLib::Module m = { TE_SERIALIZATION_MODULE_NAME,
                         te::serialize::Module::initialize,
                         te::serialize::Module::finalize
                       };

// initialize TerraLib singleton
  TerraLib::getInstance().add(m);

// it initializes the Translator support for the TerraLib Map Tools module
  TE_ADD_TEXT_DOMAIN(TE_SERIALIZATION_TEXT_DOMAIN, TE_SERIALIZATION_TEXT_DOMAIN_DIR, "UTF-8");
}

te::serialize::Module::~Module()
{
  TerraLib::getInstance().remove(TE_SERIALIZATION_MODULE_NAME);
}

void te::serialize::Module::initialize()
{
  TE_LOG_TRACE(TR_SERIALIZATION("TerraLib Serialization module initialized!"));
}

void te::serialize::Module::finalize()
{
  TE_LOG_TRACE(TR_SERIALIZATION("TerraLib Serialization module finalized!"));
}

