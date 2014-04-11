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
  \file Property.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "Property.h"
#include <vector>

te::layout::Property::Property() :
  m_name("unknown"),
  m_id("unknown"),
  m_editable(true),
  m_type(DataTypeNone)
{

}

te::layout::Property::~Property()
{

}

std::string te::layout::Property::getName()
{
  return m_name;
}

void te::layout::Property::setName( std::string name )
{
  m_name = name;
}

te::layout::LayoutPropertyDataType te::layout::Property::getType()
{
  return m_type;
}

std::string te::layout::Property::getId()
{
  return m_id;
}

void te::layout::Property::setId( std::string id )
{
  m_id = id;
}

te::layout::Variant te::layout::Property::getValue()
{
  return m_value;
}

void te::layout::Property::addOption(Variant variant)
{

}

void te::layout::Property::removeOption(Variant variant)
{

}

void te::layout::Property::setOptionChoice(Variant variant)
{
  m_variants.push_back(variant); 
}

te::layout::Variant te::layout::Property::getOptionByCurrentChoice()
{
  return m_currentChoice;
}

std::vector<te::layout::Variant> te::layout::Property::getOptionChoices()
{
  return m_variants;
}

bool te::layout::Property::isEditable()
{
  return m_editable;
}

void te::layout::Property::setEditable( bool editable )
{
  m_editable = editable;
}

void te::layout::Property::addDependentProperty( Property property )
{
  m_dependentProperty.push_back(property);
}

void te::layout::Property::removeDependentProperty( Property property )
{

}

void te::layout::Property::addSubProperty( Property property )
{
  m_subProperty.push_back(property);
}

void te::layout::Property::removeSubProperty( Property property )
{

}

std::vector<te::layout::Property> te::layout::Property::getSubProperty()
{
  return m_subProperty;
}
