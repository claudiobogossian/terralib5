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
  \file terralib/se/Module.cpp
   
  \brief This singleton defines the TerraLib Symbology Encoding module entry.
*/

// TerraLib
#include "../common/Logger.h"
#include "../common/TerraLib.h"
#include "../common/Translator.h"
#include "Config.h"
#include "Module.h"

const te::se::Module& sm_module = te::se::Module::getInstance();

te::se::Module::Module()
{
  TerraLib::Module m = { TE_SE_MODULE_NAME,
                         te::se::Module::initialize,
                         te::se::Module::finalize
                       };

// initialize TerraLib singleton
  TerraLib::getInstance().add(m);
}

te::se::Module::~Module()
{
  TerraLib::getInstance().remove(TE_SE_MODULE_NAME);
}

void te::se::Module::initialize()
{
  TE_LOG_TRACE(TE_TR("TerraLib Symbology Encoding module initialized!"));
}

void te::se::Module::finalize()
{
  TE_LOG_TRACE(TE_TR("TerraLib Symbology Encoding module finalized!"));
}

