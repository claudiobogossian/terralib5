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
   
  \brief This singleton defines the TerraLib Filter Encoding module entry.
*/

// TerraLib
#include "../common/Logger.h"
#include "../common/TerraLib.h"
#include "../common/Translator.h"
#include "ComparisonOperators.h"
#include "GeometryOperands.h"
#include "Module.h"
#include "SpatialOperator.h"

const te::fe::Module& sm_module = te::fe::Module::getInstance();

te::fe::Module::Module()
{
  TerraLib::Module m = { TE_FILTERENCODING_MODULE_NAME,
                         te::fe::Module::initialize,
                         te::fe::Module::finalize
                       };

// initialize TerraLib singleton
  TerraLib::getInstance().add(m);
}

te::fe::Module::~Module()
{
  TerraLib::getInstance().remove(TE_FILTERENCODING_MODULE_NAME);
}

void te::fe::Module::initialize()
{
// initializes the basic binary operator list
  te::fe::GeometryOperands::loadValidGeometryOperandsList();
  te::fe::SpatialOperator::loadValidOperatorList();
  te::fe::ComparisonOperators::loadValidOperatorList();

  TE_LOG_TRACE(TE_TR("TerraLib Filter Encoding module initialized!"));
}

void te::fe::Module::finalize()
{
// finalizes the basic binary operator list
  te::fe::GeometryOperands::clearValidGeometryOperandsList();
  te::fe::SpatialOperator::clearValidOperatorList();
  te::fe::ComparisonOperators::clearValidOperatorList();

  TE_LOG_TRACE(TE_TR("TerraLib Filter Encoding module finalized!"));
}



