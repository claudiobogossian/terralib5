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
  \file terralib/dataaccess/query/Sub.cpp

  \brief The subtraction operator.
*/

// TerraLib
#include "Sub.h"

te::da::Sub::Sub(const Sub& rhs)
  : BinaryOp(rhs)
{
}

te::da::Sub& te::da::Sub::operator=(const Sub& rhs)
{
  BinaryOp::operator=(rhs);
  return *this;
}

te::da::Expression* te::da::Sub::clone() const
{
  return new Sub(*this);
}

