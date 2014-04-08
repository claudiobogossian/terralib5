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
  \file LayoutJSONManager.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "LayoutSerialization.h"

te::layout::LayoutSerialization::LayoutSerialization() :
  m_loadPath(""),
  m_serializationPath(""),
  m_rootKey("Unknown")
{

}

te::layout::LayoutSerialization::~LayoutSerialization()
{

}

void te::layout::LayoutSerialization::setSerializationPath( std::string path )
{
  m_serializationPath = path;
}

std::string te::layout::LayoutSerialization::getSerializationPath()
{
  return m_serializationPath;
}

std::string te::layout::LayoutSerialization::getLoadPath()
{
  return m_loadPath;
}

void te::layout::LayoutSerialization::setRootKey( std::string rootKey )
{
  m_rootKey = rootKey;
}

std::string te::layout::LayoutSerialization::getRootKey()
{
  return m_rootKey;
}