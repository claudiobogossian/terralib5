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
  \file ObjectId.cpp
  
  \brief An object identifier is meant to represent a unique identifier for an object instance within the context of the web service or database that is serving the object.  
 */

// TerraLib
#include "ObjectId.h"

// STL
#include <cassert>

te::fe::ObjectId::ObjectId(const char* id, OidType oidType)
  : m_id(id),
    m_oidType(oidType)
{
  assert(m_id.empty() == false);
}

te::fe::ObjectId::~ObjectId()
{
}

void te::fe::ObjectId::setId(const std::string& id)
{
  assert(id.empty() == false);
  m_id = id;
}

const std::string& te::fe::ObjectId::getId() const
{
  return m_id;
}

void te::fe::ObjectId::setOidType(OidType t)
{
  m_oidType = t;
}

te::fe::ObjectId::OidType te::fe::ObjectId::getOidType() const
{
  return m_oidType;
}