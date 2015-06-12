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
  \file terralib/dataaccess/query/LiteralString.cpp

  \brief A class that models a Literal String value.
*/

// TerraLib
#include "../../datatype/SimpleData.h"
#include "LiteralString.h"

te::da::LiteralString::LiteralString(const std::string& s)
  : Literal(new te::dt::String(s))
{
}

te::da::LiteralString::LiteralString(const LiteralString& rhs)
  : Literal(rhs)
{
}

te::da::LiteralString& te::da::LiteralString::operator=(const LiteralString& rhs)
{
  Literal::operator=(rhs);

  return *this;
}

te::da::Expression* te::da::LiteralString::clone() const
{
  return new LiteralString(*this);
}

