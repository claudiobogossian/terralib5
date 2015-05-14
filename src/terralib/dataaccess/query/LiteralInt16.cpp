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
  \file terralib/dataaccess/query/LiteralInt16.cpp

  \brief A class that models a Literal for a integer value.
 */

// TerraLib
#include "../../datatype/SimpleData.h"
#include "LiteralInt16.h"

te::da::LiteralInt16::LiteralInt16(boost::int16_t i)
  : Literal(new te::dt::Int16(i))
{
}

te::da::LiteralInt16::LiteralInt16(const LiteralInt16& rhs)
  : Literal(rhs)
{
}

te::da::LiteralInt16& te::da::LiteralInt16::operator=(const LiteralInt16& rhs)
{
  Literal::operator=(rhs);

  return *this;
}

te::da::Expression* te::da::LiteralInt16::clone() const
{
  return new LiteralInt16(*this);
}

