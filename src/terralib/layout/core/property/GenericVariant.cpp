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

te::layout::GenericVariant::GenericVariant()
{

}

te::layout::GenericVariant::~GenericVariant()
{
  clear();
}

std::vector<std::string> te::layout::GenericVariant::toStringVector()
{
  return m_vString;
}

const std::list<te::map::AbstractLayerPtr>& te::layout::GenericVariant::toLayerList() const
{
  return m_listLayer;
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

  if(m_type == dataType->getDataTypeStringVector())
  {
    return m_vString.empty();
  }
  if(m_type == dataType->getDataTypeLayerList())
  {
    return m_listLayer.empty();
  }

  return result;
}

void te::layout::GenericVariant::clear()
{
  m_vString.clear();
  m_listLayer.clear();
}

void te::layout::GenericVariant::fromPtree( boost::property_tree::ptree tree )
{

}

std::string te::layout::GenericVariant::vectorToString()
{
  std::string s;
  return s;
}

std::string te::layout::GenericVariant::listToString()
{
  std::string s;
  return s;
}





