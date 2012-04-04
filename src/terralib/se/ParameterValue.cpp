/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file ParameterValue.cpp
  
  \brief The ParameterValueType uses WFS-Filter expressions to give values for SE graphic parameters.
 */

// TerraLib
#include "../common/STLUtils.h"
#include "../fe/Expression.h"
#include "ParameterValue.h"

te::se::ParameterValue::Parameter::Parameter()
  : m_mixedData(0),
    m_expression(0)
{              
}

te::se::ParameterValue::Parameter::~Parameter()
{
  delete m_mixedData;
  delete m_expression;
}

te::se::ParameterValue::ParameterValue()
{
}

te::se::ParameterValue::~ParameterValue()
{
  te::common::FreeContents(m_parameters);
}

void te::se::ParameterValue::add(Parameter* p)
{
  m_parameters.push_back(p);
}

