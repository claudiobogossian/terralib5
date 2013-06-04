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
  \file terralib/dataaccess/core/query/Expression.cpp
  
  \brief This is an abstract class that models a query expression.
*/

// TerraLib
#include "And.h"
#include "EqualTo.h"
#include "Expression.h"
#include "GreaterThan.h"
#include "GreaterThanOrEqualTo.h"
#include "LessThan.h"
#include "LessThanOrEqualTo.h"
#include "Not.h"
#include "NotEqualTo.h"
#include "Or.h"

te::da::core::Expression* operator==(const te::da::core::Expression& e1, const te::da::core::Expression& e2)
{
  return new te::da::core::EqualTo(e1.clone(), e2.clone());
}

te::da::core::Expression* operator!=(const te::da::core::Expression& e1, const te::da::core::Expression& e2)
{
  return new te::da::core::NotEqualTo(e1.clone(), e2.clone());
}

te::da::core::Expression* operator&&(const te::da::core::Expression& e1, const te::da::core::Expression& e2)
{
  return new te::da::core::And(e1.clone(), e2.clone());
}

te::da::core::Expression* operator||(const te::da::core::Expression& e1, const te::da::core::Expression& e2)
{
  return new te::da::core::Or(e1.clone(), e2.clone());
}

te::da::core::Expression* operator~(const te::da::core::Expression& e)
{
  return new te::da::core::Not(e.clone());
}

te::da::core::Expression* operator>(const te::da::core::Expression& e1, const te::da::core::Expression& e2)
{
  return new te::da::core::GreaterThan(e1.clone(), e2.clone());
}

te::da::core::Expression* operator>=(const te::da::core::Expression& e1, const te::da::core::Expression& e2)
{
  return new te::da::core::GreaterThanOrEqualTo(e1.clone(), e2.clone());
}

te::da::core::Expression* operator<(const te::da::core::Expression& e1, const te::da::core::Expression& e2)
{
  return new te::da::core::LessThan(e1.clone(), e2.clone());
}

te::da::core::Expression* operator<=(const te::da::core::Expression& e1, const te::da::core::Expression& e2)
{
  return new te::da::core::LessThanOrEqualTo(e1.clone(), e2.clone());
}

