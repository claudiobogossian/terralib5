/*  Copyright (C) 2011-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/serialization/se/ParameterValue.cpp
 
  \brief Support for ParameterValue serialization.
*/

// TerraLib
#include "../../se/ParameterValue.h"
#include "../../xml/Reader.h"
#include "../../xml/Writer.h"
#include "../fe/Expression.h"
#include "ParameterValue.h"

// STL
#include <cassert>
#include <memory>

te::se::ParameterValue* te::serialize::ReadParameterValue(te::xml::Reader& reader)
{
  std::auto_ptr<te::se::ParameterValue> param(new te::se::ParameterValue);

  // Expression TODO: (n's expressions?)
  te::se::ParameterValue::Parameter* p = new te::se::ParameterValue::Parameter;
  p->m_expression = Expression::getInstance().read(reader);
  
  // TODO: and mixed data?!
  
  param->add(p);

  return param.release();
}

void te::serialize::Save(const te::se::ParameterValue* p, te::xml::Writer& writer)
{
  if(p == 0)
    return;

  std::size_t n = p->getNParameters();
  for(std::size_t i = 0; i < n; ++i)
  {
    const te::se::ParameterValue::Parameter* param = p->getParameter(i);
    Expression::getInstance().write(param->m_expression, writer);
  }
}
