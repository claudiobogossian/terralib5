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
  \file GenericVariant.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "GenericVariant.h"
#include "../enum/Enums.h"

// STL
#include <sstream>
#include <string>  
#include <exception>
#include <stdexcept>
#include <cctype>
#include <iostream>
#include <stdlib.h>

te::layout::GenericVariant::GenericVariant()
{

}

te::layout::GenericVariant::GenericVariant(EnumType* type) :
  m_type(type)
{
 
}

te::layout::GenericVariant::~GenericVariant()
{
  clear();
}

void te::layout::GenericVariant::setVector( std::vector<boost::any> value )
{
  m_data = value;
}

std::vector<std::string> te::layout::GenericVariant::toString()
{
  return m_vString;
}

std::vector<double> te::layout::GenericVariant::toDouble()
{
  return m_vDouble;
}

std::vector<int> te::layout::GenericVariant::toInt()
{
  return m_vInt;
}

std::vector<long> te::layout::GenericVariant::toLong()
{
  return m_vLong;
}

std::vector<float> te::layout::GenericVariant::toFloat()
{
  return m_vFloat;
}

std::vector<bool> te::layout::GenericVariant::toBool()
{
  return m_vBool;
}

std::vector<te::color::RGBAColor> te::layout::GenericVariant::toColor()
{
  return m_vRGBColor;
}

std::vector<te::layout::Font> te::layout::GenericVariant::toFont()
{
  return m_vFont;
}

std::vector<te::map::AbstractLayerPtr> te::layout::GenericVariant::toLayer()
{
  return m_vLayer;
}

std::vector<boost::any> te::layout::GenericVariant::toAny()
{
  return m_data;
}

std::string te::layout::GenericVariant::convertToString()
{
  std::string s;
  return s;
}

bool te::layout::GenericVariant::isEmpty()
{
  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  if(!m_type || !dataType)
  {
    return true;
  }

  bool result = true;

  if(m_type == dataType->getDataTypeString())
  {
    return m_vString.empty();
  }
  else if(m_type == dataType->getDataTypeDouble())
  {
    return m_vDouble.empty();
  }
  else if(m_type == dataType->getDataTypeInt())
  {
    return m_vInt.empty();
  }
  else if(m_type == dataType->getDataTypeLong())
  {
    return m_vLong.empty();
  }
  else if(m_type == dataType->getDataTypeFloat())
  {
    return m_vFloat.empty();
  }
  else if(m_type == dataType->getDataTypeBool())
  {
    return m_vBool.empty();
  }
  else if(m_type == dataType->getDataTypeColor())
  {
    return m_vRGBColor.empty();
  }
  else if(m_type == dataType->getDataTypeFont())
  {
    return m_vFont.empty();
  }
  else if(m_type == dataType->getDataTypeLayer())
  {
    return m_vLayer.empty();
  }

  return result;
}

void te::layout::GenericVariant::clear()
{
  m_data.clear();
  m_vString.clear();
  m_vDouble.clear();      
  m_vInt.clear();
  m_vLong.clear();
  m_vFloat.clear();
  m_vBool.clear();
  m_vRGBColor.clear();
  m_vFont.clear();
  m_vLayer.clear();
}

void te::layout::GenericVariant::fromPtree( boost::property_tree::ptree tree )
{

}

std::string te::layout::GenericVariant::vectorToString()
{
  std::string s;
  return s;
}



