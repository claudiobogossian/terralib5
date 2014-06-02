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
  \file terralib/dataaccess/query/SQLDialect.cpp

  \brief It represents the SQL query dialect accepted by a given data source.
*/

// TerraLib
#include "../../common/STLUtils.h"
#include "../../common/Translator.h"
#include "../Exception.h"
#include "SQLDialect.h"
#include "SQLFunctionEncoder.h"

te::da::SQLDialect::~SQLDialect()
{
  te::common::FreeContents(m_regFunctions);
}

void te::da::SQLDialect::insert(const std::string& funcName, SQLFunctionEncoder* encoder)
{
  if(te::common::GetPValue(m_regFunctions, funcName) != 0)
    throw Exception(TE_TR("There is already a function registered with the given name!"));

  m_regFunctions.insert(std::map<std::string, SQLFunctionEncoder*>::value_type(funcName, encoder));
}

const te::da::SQLFunctionEncoder* te::da::SQLDialect::find(const std::string& funcName) const
{
  return te::common::GetPValue(m_regFunctions, funcName);
}

