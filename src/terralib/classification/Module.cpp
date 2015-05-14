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
  \file terralib/classification/Module.cpp

  \brief This singleton defines the TerraLib Classification module entry.
*/

// TerraLib
#include "../common/Logger.h"
#include "../common/TerraLib.h"
#include "../common/Translator.h"
#include "Config.h"
#include "Module.h"

const te::cl::Module& sm_module = te::cl::Module::getInstance();

te::cl::Module::Module()
{
  TerraLib::Module m = { TE_CL_MODULE_NAME,
                         te::cl::Module::initialize,
                         te::cl::Module::finalize
                       };

// initialize TerraLib singleton
  TerraLib::getInstance().add(m);
}

te::cl::Module::~Module()
{
  TerraLib::getInstance().remove(TE_CL_MODULE_NAME);
}

void te::cl::Module::initialize()
{
  TE_LOG_TRACE(TE_TR("TerraLib Classification module initialized!"));
}

void te::cl::Module::finalize()
{
  TE_LOG_TRACE(TE_TR("TerraLib Classification module finalized!"));
}
