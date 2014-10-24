/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/datatype/Property.cpp

  \brief It models a property definition.
*/

// TerraLib
#include "Property.h"

te::dt::Property::Property(const std::string& name,
                           int datatype,
                           unsigned int id,
                           Property* parent,
                           const std::string& dsName)
  : m_parent(parent),
    m_id(id),
    m_type(datatype),
    m_name(name),
    m_datasetName(dsName)
{
}

te::dt::Property::Property(const Property& rhs)
  : m_parent(0),
    m_id(rhs.m_id),
    m_type(rhs.m_type),
    m_name(rhs.m_name),
    m_datasetName(rhs.m_datasetName)
{  
}

te::dt::Property& te::dt::Property::operator=(const Property& rhs)
{
  if(this != &rhs)
  {
    m_parent = 0;
    m_id = rhs.m_id;
    m_type = rhs.m_type;
    m_name = rhs.m_name;
    m_datasetName = rhs.m_datasetName;
  }

  return *this;
}



