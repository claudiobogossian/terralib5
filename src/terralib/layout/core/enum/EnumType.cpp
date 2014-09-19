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
  \file EnumType.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "EnumType.h"
#include <ios> 
#include <iostream>
#include "../../../common/StringUtils.h"


te::layout::EnumType::EnumType( int id, std::string name ) :
  m_id(id),
  m_label("")
{
  m_name = te::common::Convert2UCase(name);
}

te::layout::EnumType::~EnumType()
{

}

int te::layout::EnumType::getId()
{
  return m_id;
}

std::string te::layout::EnumType::getName()
{
  return m_name;
}

void te::layout::EnumType::setName( std::string name )
{
  m_name = te::common::Convert2UCase(name);
}

std::string te::layout::EnumType::getLabel()
{
  return m_label;
}

void te::layout::EnumType::setLabel( std::string label )
{
  m_label = label;
}
