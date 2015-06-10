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
  \file terralib/dataaccess/query/UnaryOpEncoder.cpp

  \brief A query encoder for unary operator expressions.
*/

// TerraLib
#include "Function.h"
#include "SQLVisitor.h"
#include "UnaryOpEncoder.h"

// STL
#include <cassert>

void te::da::UnaryOpEncoder::toSQL(const Function& f,
                                   std::string& buff,
                                   SQLVisitor& v) const
{
  assert(f.getNumArgs() == 1);
  //buff += "(";
  buff += m_name;
  f[0]->accept(v);
  //buff += ")";
}

