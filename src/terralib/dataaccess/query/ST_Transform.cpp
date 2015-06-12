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
  \file terralib/dataaccess/query/ST_Transform.cpp

  \brief Spatial reference system transformation function.
*/

// TerraLib
#include "FunctionNames.h"
#include "LiteralInt32.h"
#include "ST_Transform.h"

te::da::ST_Transform::ST_Transform(Expression* arg, int srid)
  : Function(FunctionNames::sm_ST_Transform)
{
  m_args.push_back(arg);
  m_args.push_back(new LiteralInt32(srid));
}

te::da::ST_Transform::ST_Transform(const Expression& arg, int srid)
  : Function(FunctionNames::sm_ST_Transform)
{
  m_args.push_back(arg.clone());
  m_args.push_back(new LiteralInt32(srid));
}

te::da::ST_Transform::ST_Transform(const ST_Transform& rhs)
  : Function(rhs)
{
}

te::da::ST_Transform& te::da::ST_Transform::operator=(const ST_Transform& rhs)
{
  Function::operator=(rhs);
  return *this;
}

te::da::Expression* te::da::ST_Transform::clone() const
{
  return new ST_Transform(*this);
}

