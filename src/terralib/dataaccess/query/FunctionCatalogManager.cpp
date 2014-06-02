/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/dataaccess/query/FunctionCatalogManager.cpp

  \brief A FunctionCatalogManager is a singleton that can be used to manage function catalogs of data source implementations.
*/

// TerraLib
#include "../../common/STLUtils.h"
#include "../../common/Translator.h"
#include "../Exception.h"
#include "FunctionCatalog.h"
#include "FunctionCatalogManager.h"
#include "FunctionDefn.h"

te::da::FunctionCatalogManager::~FunctionCatalogManager()
{
  te::common::FreeContents(m_fcatalogMap);
}

void te::da::FunctionCatalogManager::insert(const std::string& dsType, FunctionCatalog* fcatalog)
{
  if(te::common::GetPValue(m_fcatalogMap, dsType) != 0)
    throw Exception(TE_TR("There is already a function catalog resgistered for the data source type!"));

  m_fcatalogMap.insert(std::map<std::string, FunctionCatalog*>::value_type(dsType, fcatalog));
}

te::da::FunctionCatalog* te::da::FunctionCatalogManager::find(const std::string& dsType)
{
  return te::common::GetPValue(m_fcatalogMap, dsType);
}

