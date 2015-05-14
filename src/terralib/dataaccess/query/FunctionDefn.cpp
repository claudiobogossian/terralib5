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
  \file terralib/dataaccess/query/FunctionDefn.cpp

  \brief The definition of a function that can be used in a query object.
*/

// TerraLib
#include "../../common/STLUtils.h"
#include "FunctionDefn.h"
#include "FunctionParameter.h"

// STL
#include <cassert>

te::da::FunctionDefn::FunctionDefn(const std::string& name,
                                   int retValType,
                                   bool isAggregate,
                                   std::string* description)
  : m_name(name),
    m_retValType(retValType),
    m_isAggregate(isAggregate),
    m_description(description)
{
}

te::da::FunctionDefn::~FunctionDefn()
{
  te::common::FreeContents(m_params);
  delete m_description;
}

void te::da::FunctionDefn::setDescription(std::string* description)
{
  delete m_description;
  m_description = description;
}

