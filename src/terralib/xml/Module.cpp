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
  \file terralib/xml/Module.cpp
   
  \brief This singleton defines the TerraLib XML-API module.
*/

// TerraLib
#include "../common/Logger.h"
#include "../common/TerraLib.h"
#include "../common/Translator.h"
#include "Config.h"
#include "Module.h"

const te::xml::Module& sm_module = te::xml::Module::getInstance();

te::xml::Module::Module()
{
  TerraLib::Module m = { TE_XML_MODULE_NAME,
                         te::xml::Module::initialize,
                         te::xml::Module::finalize
                       };

// initialize TerraLib singleton
  TerraLib::getInstance().add(m);
}

te::xml::Module::~Module()
{
  TerraLib::getInstance().remove(TE_XML_MODULE_NAME);
}

void te::xml::Module::initialize()
{
  TE_LOG_TRACE(TE_TR("TerraLib XML module initialized!"));
}

void te::xml::Module::finalize()
{
  TE_LOG_TRACE(TE_TR("TerraLib XML module finalized!"));
}

