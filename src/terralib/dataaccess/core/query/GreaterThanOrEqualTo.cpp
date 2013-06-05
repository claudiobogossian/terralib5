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
  \file terralib/dataaccess/core/query/GreaterThanOrEqualTo.cpp

  \brief It models the inequality operator greater than or equal to (>=) - syntatic-suggar.
*/

// TerraLib
#include "GreaterThanOrEqualTo.h"

te::da::core::GreaterThanOrEqualTo::GreaterThanOrEqualTo(const GreaterThanOrEqualTo& rhs)
  : BinaryOp(rhs)
{
}

te::da::core::GreaterThanOrEqualTo& te::da::core::GreaterThanOrEqualTo::operator=(const GreaterThanOrEqualTo& rhs)
{
  BinaryOp::operator=(rhs);
  return *this;
}

te::da::core::Expression* te::da::core::GreaterThanOrEqualTo::clone() const
{
  return new GreaterThanOrEqualTo(*this);
}
