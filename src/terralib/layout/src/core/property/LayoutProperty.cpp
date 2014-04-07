/*  Copyright (C) 2001-2014 National Institute For Space Research (INPE) - Brazil.

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
#include "LayoutProperty.h"
#include <vector>

te::layout::LayoutProperty::LayoutProperty() :
  m_name("unknown"),
  m_id("unknown"),
  m_editable(true),
  m_type(DataTypeNone)
{

}

te::layout::LayoutProperty::~LayoutProperty()
{

}

std::string te::layout::LayoutProperty::getName()
{
  return m_name;
}

void te::layout::LayoutProperty::setName( std::string name )
{
  m_name = name;
}

te::layout::LayoutPropertyDataType te::layout::LayoutProperty::getType()
{
  return m_type;
}

std::string te::layout::LayoutProperty::getId()
{
  return m_id;
}

void te::layout::LayoutProperty::setId( std::string id )
{
  m_id = id;
}

te::layout::LayoutVariant te::layout::LayoutProperty::getValue()
{
  return m_value;
}

void te::layout::LayoutProperty::addOption(LayoutVariant variant)
{

}

void te::layout::LayoutProperty::removeOption(LayoutVariant variant)
{

}

void te::layout::LayoutProperty::setOptionChoice(LayoutVariant variant)
{
  m_variants.push_back(variant); 
}

te::layout::LayoutVariant te::layout::LayoutProperty::getOptionByCurrentChoice()
{
  return m_currentChoice;
}

std::vector<te::layout::LayoutVariant> te::layout::LayoutProperty::getOptionChoices()
{
  return m_variants;
}

bool te::layout::LayoutProperty::isEditable()
{
  return m_editable;
}

void te::layout::LayoutProperty::setEditable( bool editable )
{
  m_editable = editable;
}

void te::layout::LayoutProperty::addDependentProperty( LayoutProperty property )
{
  m_dependentProperty.push_back(property);
}

void te::layout::LayoutProperty::removeDependentProperty( LayoutProperty property )
{

}

void te::layout::LayoutProperty::addSubProperty( LayoutProperty property )
{
  m_subProperty.push_back(property);
}

void te::layout::LayoutProperty::removeSubProperty( LayoutProperty property )
{

}

std::vector<te::layout::LayoutProperty> te::layout::LayoutProperty::getSubProperty()
{
  return m_subProperty;
}
