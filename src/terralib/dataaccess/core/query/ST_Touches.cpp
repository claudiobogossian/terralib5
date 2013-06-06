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
  \file terralib/dataaccess/core/query/ST_Touches.cpp

  \brief Spatial touches operator.
*/

// TerraLib
#include "ST_Touches.h"

te::da::core::ST_Touches::ST_Touches(const ST_Touches& rhs)
  : BinaryFunction(rhs)
{
}

te::da::core::ST_Touches& te::da::core::ST_Touches::operator=(const ST_Touches& rhs)
{
  BinaryFunction::operator=(rhs);
  return *this;
}

te::da::core::Expression* te::da::core::ST_Touches::clone() const
{
  return new ST_Touches(*this);
}
