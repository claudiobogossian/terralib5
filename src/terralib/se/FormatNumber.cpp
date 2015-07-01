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
  \file terralib/se/FormatNumber.cpp
  
  \brief Numeric formatting function.
*/

// TerraLib
#include "FormatNumber.h"
#include "ParameterValue.h"

te::se::FormatNumber::FormatNumber()
  : m_numericValue(0),
    m_decimalPoint("."),
    m_groupingSeparator(",")
{
}

te::se::FormatNumber::~FormatNumber()
{
  delete m_numericValue;
}

void te::se::FormatNumber::setNumericValue(ParameterValue* v)
{
  delete m_numericValue;
  m_numericValue = v;
}

void te::se::FormatNumber::setPattern(const std::string& p)
{
  m_pattern = p;
}

void te::se::FormatNumber::setNegativePattern(const std::string& np)
{
  m_negativePattern = np;
}

void te::se::FormatNumber::setDecimalPoint(const std::string& dp)
{
  m_decimalPoint = dp;
}

void te::se::FormatNumber::setGroupingSeparator(const std::string& gs)
{
  m_groupingSeparator = gs;
}
