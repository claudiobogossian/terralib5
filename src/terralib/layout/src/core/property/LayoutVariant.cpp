/*  Copyright (C) 2014-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file LayoutProperty.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "LayoutVariant.h"

te::layout::LayoutVariant::LayoutVariant() :
  m_sValue("unknown"),
  m_dValue(0.),
  m_iValue(0),
  m_lValue(0),
  m_fValue(0.),
  m_type(DataTypeNone),
  m_null(true)
{

}

te::layout::LayoutVariant::LayoutVariant(te::layout::LayoutPropertyDataType type, const void* valueCopy) :
  m_sValue("unknown"),
  m_dValue(0.),
  m_iValue(0),
  m_lValue(0),
  m_fValue(0.),
  m_type(type),
  m_null(true)
{
  if(valueCopy)
  {
    convertValue(valueCopy);
  }
}

te::layout::LayoutVariant::~LayoutVariant()
{
  
}

te::layout::LayoutPropertyDataType te::layout::LayoutVariant::getType()
{
  return m_type;
}

void te::layout::LayoutVariant::convertValue( const void* valueCopy )
{
  void* value = const_cast<void*>(valueCopy);
  bool null = false;

  //init variables
  std::string* sp = 0;
  double* dValue = 0;
  float* fValue = 0;
  long* lValue = 0;
  int* iValue = 0;

  switch(m_type)
  {
  case DataTypeString:
    // Cast it back to a string pointer.
    sp = static_cast<std::string*>(value);
    if(!sp)
    {
      null = true;
      //return throw_exception("Cast failure! Wrong type.");
    }
    m_sValue = *sp;
    break;
  case DataTypeDouble:
    dValue = static_cast<double*>(value);
    if(!dValue)
    {
      null = true;
    }
    m_dValue = *dValue;
    break;
  case DataTypeFloat:
    fValue = static_cast<float*>(value);
    if(!fValue)
    {
      null = true;
    }
    m_fValue = *fValue;
    break;
  case DataTypeLong:
    lValue = static_cast<long*>(value);
    if(!lValue)
    {
      null = true;
    }
    m_lValue = *lValue;
    break;
  case DataTypeInt:
    iValue = static_cast<int*>(value);
    if(!iValue)
    {
      null = true;
    }
    m_iValue = *iValue;
    break;
  case DataTypeNone:
    null = true;
    break;
  }

  m_null = null;
}

std::string te::layout::LayoutVariant::toString()
{
  return m_sValue;
}

double te::layout::LayoutVariant::toDouble()
{
  return m_dValue;
}

int te::layout::LayoutVariant::toInt()
{
  return m_iValue;
}

long te::layout::LayoutVariant::toLong()
{
  return m_lValue;
}

float te::layout::LayoutVariant::toFloat()
{
  return m_fValue;
}

bool te::layout::LayoutVariant::isNull()
{
  return m_null;
}
