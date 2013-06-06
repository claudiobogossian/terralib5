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
  \file terralib/dataaccess/core/query/IsNull.cpp

  \brief It reports if a value is NULL.
*/

// TerraLib
#include "IsNull.h"

te::da::core::IsNull::IsNull(const IsNull& rhs)
  : UnaryFunction(rhs)
{
}

te::da::core::IsNull& te::da::core::IsNull::operator=(const IsNull& rhs)
{
  UnaryFunction::operator=(rhs);
  return *this;
}

te::da::core::Expression* te::da::core::IsNull::clone() const
{
  return new IsNull(*this);
}
