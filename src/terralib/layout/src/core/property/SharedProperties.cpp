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
  \file SharedProperties.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "SharedProperties.h"


te::layout::SharedProperties::SharedProperties() :
  m_name("name"),
  m_id("id"),
  m_mapName("map_name")
{

}

te::layout::SharedProperties::~SharedProperties( void )
{

}

std::string te::layout::SharedProperties::getName()
{
  return m_name;
}

std::string te::layout::SharedProperties::getId()
{
  return m_id;
}

std::string te::layout::SharedProperties::getMapName()
{
  return m_mapName;
}
