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
  \file terralib/dataaccess/query/FunctionEncoder.cpp

  \brief A query encoder for general function expressions.
*/

// TerraLib
#include "FunctionEncoder.h"
#include "Function.h"
#include "SQLVisitor.h"

// STL
#include <cassert>
#include <string>

void te::da::FunctionEncoder::toSQL(const Function& f,
                                    std::string& buff,
                                    SQLVisitor& v) const
{
  size_t size = f.getNumArgs();

  if(m_name.compare("st_dump") != 0 && m_name.compare("st_dumprings") != 0)
  {
    buff += m_name;
    buff += "(";

    for(size_t i = 0; i < size; ++i)
    {
      if(i != 0)
        buff += ", ";

      assert(f[i]);
      f[i]->accept(v);
    }

    buff += ")";
  }
  else
  {
    buff += "(";
    buff += m_name;
    buff += "(";

    for(size_t i = 0; i < size; ++i)
    {
      if(i != 0)
        buff += ", ";

      assert(f[i]);
      f[i]->accept(v);
    }

    buff += ")).geom";
  }
}

