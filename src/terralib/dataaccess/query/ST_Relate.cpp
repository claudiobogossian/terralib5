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
  \file terralib/dataaccess/query/ST_Relate.cpp

  \brief Spatial relation function.
*/

// TerraLib
#include "FunctionNames.h"
#include "LiteralString.h"
#include "ST_Relate.h"

te::da::ST_Relate::ST_Relate(Expression* g1, Expression* g2, LiteralString* intersectionMatrix)
  : Function(FunctionNames::sm_ST_Relate)
{
  m_args.push_back(g1);
  m_args.push_back(g2);
  
  if(intersectionMatrix)
    m_args.push_back(intersectionMatrix);
}

te::da::ST_Relate::ST_Relate(const Expression& g1, const Expression& g2, const std::string& intersectionMatrix)
  : Function(FunctionNames::sm_ST_Relate)
{
  m_args.push_back(g1.clone());
  m_args.push_back(g2.clone());
  
  if(!intersectionMatrix.empty())
    m_args.push_back(new LiteralString(intersectionMatrix));
}

te::da::ST_Relate::ST_Relate(const ST_Relate& rhs)
  : Function(rhs)
{
}

te::da::ST_Relate& te::da::ST_Relate::operator=(const ST_Relate& rhs)
{
  Function::operator=(rhs);
  return *this;
}

te::da::Expression* te::da::ST_Relate::clone() const
{
  return new ST_Relate(*this);
}

