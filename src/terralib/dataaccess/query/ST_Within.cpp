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
  \file terralib/dataaccess/query/ST_Within.cpp

  \brief Spatial within operator.
*/

// TerraLib
#include "ST_Within.h"

te::da::ST_Within::ST_Within(const ST_Within& rhs)
  : BinaryFunction(rhs)
{
}

te::da::ST_Within& te::da::ST_Within::operator=(const ST_Within& rhs)
{
  BinaryFunction::operator=(rhs);
  return *this;
}

te::da::Expression* te::da::ST_Within::clone() const
{
  return new ST_Within(*this);
}

