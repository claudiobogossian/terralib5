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
  \file terralib/dataaccess/query/Add.cpp

  \brief The addition operator - syntatic-suggar.
*/

// TerraLib
#include "Add.h"

te::da::Add::Add(const Add& rhs)
  : BinaryOp(rhs)
{
}

te::da::Add& te::da::Add::operator=(const Add& rhs)
{
  BinaryOp::operator=(rhs);

  return *this;
}

te::da::Expression* te::da::Add::clone() const
{
  return new Add(*this);
}

