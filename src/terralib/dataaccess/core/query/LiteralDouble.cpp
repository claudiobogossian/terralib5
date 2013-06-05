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
  \file terralib/dataaccess/core/query/LiteralDouble.cpp

  \brief A class that models a literal for double values.
*/

// TerraLib
#include "../../../datatype/SimpleData.h"
#include "LiteralDouble.h"

te::da::core::LiteralDouble::LiteralDouble(te::dt::AbstractData* v)
  : Literal(v)
{
}

te::da::core::LiteralDouble::LiteralDouble(const te::dt::AbstractData& v)
  : Literal(v)
{
}

te::da::core::LiteralDouble::LiteralDouble(const double& v)
  : Literal(new te::dt::Double(v))
{
}

te::da::core::LiteralDouble::LiteralDouble(const LiteralDouble& rhs)
  : Literal(rhs)
{
}

te::da::core::LiteralDouble::~LiteralDouble()
{
}

te::da::core::LiteralDouble& te::da::core::LiteralDouble::operator=(const LiteralDouble& rhs)
{
  Literal::operator=(rhs);

  return *this;
}

te::da::core::Expression* te::da::core::LiteralDouble::clone() const
{
  return new LiteralDouble(*this);
}
