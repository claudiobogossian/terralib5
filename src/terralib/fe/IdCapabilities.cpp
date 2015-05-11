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
  \file IdCapabilities.cpp
  
  \brief Id capabilities include the ability to refer to elements in a GML version 3 data source using an ogc:GmlObjectId with a gml:id attribute.
 */

// TerraLib
#include "IdCapabilities.h"

te::fe::IdCapabilities::IdCapabilities()
  : m_hasEID(false),
    m_hasFID(false)
{
}

te::fe::IdCapabilities::~IdCapabilities()
{
}

const bool& te::fe::IdCapabilities::hasEID() const
{
  return m_hasEID;
}

void te::fe::IdCapabilities::enableEID()
{
  m_hasEID = true;
}

void te::fe::IdCapabilities::disableEID()
{
  m_hasEID = false;
}

const bool& te::fe::IdCapabilities::hasFID() const
{
  return m_hasFID;
}

void te::fe::IdCapabilities::enableFID()
{
  m_hasFID = true;
}

void te::fe::IdCapabilities::disableFID()
{
  m_hasFID = false;
}

