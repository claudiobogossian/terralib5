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
  \file terralib/rp/Module.cpp
   
  \brief This singleton defines the TerraLib Raster Processing module entry.
 */

// TerraLib
#include "../core/logger/Logger.h"
#include "../common/TerraLib.h"
#include "../core/translator/Translator.h"
#include "Config.h"
#include "Module.h"

const te::rp::Module& sm_module = te::rp::Module::getInstance();
std::string te::rp::Module::m_lastLogString;

te::rp::Module::Module()
{
  TerraLib::Module m = { TE_RP_MODULE_NAME,
                         te::rp::Module::initialize,
                         te::rp::Module::finalize
                       };

// initialize TerraLib singleton
  TerraLib::getInstance().add(m);
}

te::rp::Module::~Module()
{
  TerraLib::getInstance().remove(TE_RP_MODULE_NAME);
}

void te::rp::Module::initialize()
{
  TE_LOG_TRACE(TE_TR("TerraLib Raster Processing module initialized!"));
}

void te::rp::Module::finalize()
{
  TE_LOG_TRACE(TE_TR("TerraLib Raster Processing module finalized!"));
}

