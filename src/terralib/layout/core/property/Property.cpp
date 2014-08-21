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

// STL
#include <vector>
#include <algorithm>

te::layout::Property::Property() :
  m_name("unknown"),
  m_id("unknown"),
  m_label(""),
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
  if(m_options.empty())
  {
    m_currentChoice = variant;
  }

  m_options.push_back(variant);
}

void te::layout::Property::removeOption(Variant variant)
{
  for(std::vector<Variant>::iterator it = m_options.begin(); it != m_options.end(); it++)
  {
    if((*it) == variant)
    {
      m_options.erase(it);
      break;
    }
  }
}

void te::layout::Property::setOptionChoice(Variant variant)
{
  m_currentChoice = variant; 
}

te::layout::Variant te::layout::Property::getOptionByCurrentChoice()
{
  return m_currentChoice;
}

std::vector<te::layout::Variant> te::layout::Property::getOptionChoices()
{
  return m_options;;
}

bool te::layout::Property::isEditable()
{
  return m_editable;
}

void te::layout::Property::setEditable( bool editable )
{
  m_editable = editable;
}

void te::layout::Property::addSubProperty( Property property )
{
  m_subProperty.push_back(property);
}

void te::layout::Property::removeSubProperty( Property property )
{
  for(std::vector<Property>::iterator it = m_subProperty.begin(); it != m_subProperty.end(); it++)
  {
    if((*it) == property)
    {
      m_subProperty.erase(it);
      break;
    }
  }
}

std::vector<te::layout::Property> te::layout::Property::getSubProperty()
{
  return m_subProperty;
}

bool te::layout::Property::isNull()
{
  bool result = true;

  if(m_value.isNull())
  {
    if(!m_options.empty())
    {
      result = false;
    }
  }
  else
  {
    result = false;
  }

  return result;
}

bool te::layout::Property::containsSubProperty( Property subProperty )
{
  bool is_present = false;

  if(std::find(m_subProperty.begin(), m_subProperty.end(), subProperty) != m_subProperty.end())
  {
    is_present = true;
  }

  return is_present;
}

te::layout::Property te::layout::Property::containsSubProperty( std::string name )
{
  Property property;
  property.setName(name);

  if(std::find(m_subProperty.begin(), m_subProperty.end(), property) != m_subProperty.end())
  {
    std::vector<Property>::iterator it = std::find(m_subProperty.begin(), m_subProperty.end(), property);

    property = (*it);
  }
  else
    property.setName("");

  return property;
}

void te::layout::Property::clear()
{
  m_name = "unknown";
  m_id = "unknown";
  m_editable = true;
  m_type = DataTypeNone;
  m_value.clear();
  m_currentChoice.clear();
  m_options.clear();
  m_subProperty.clear();
}

void te::layout::Property::setValue( Variant variant )
{
  m_value = variant;
  m_type = variant.getType();
}

void te::layout::Property::setLabel( std::string label )
{
  m_label = label;
}

std::string te::layout::Property::getLabel()
{
  return m_label;
}
