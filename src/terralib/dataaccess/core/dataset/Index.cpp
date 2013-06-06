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

/*!H
  \file terralib/dataaccess/core/dataset/Index.cpp

  \brief It describes an index associated to a dataset schema.
*/

// TerraLib
#include "Index.h"

te::da::core::Index::Index(const std::string& name, IndexType t)
  : m_name(name),
    m_dt(0),
    m_type(t)
{
}

te::da::core::Index::Index(const std::string& name,
                           IndexType t,
                           const std::vector<std::size_t>& properties)
  : m_name(name),
    m_properties(properties),
    m_dt(0),
    m_type(t)
{
}

te::da::core::Index::Index(const Index& rhs)
  : m_name(rhs.m_name),
    m_properties(rhs.m_properties),
    m_dt(0),
    m_type(rhs.m_type)
{
}

te::da::core::Index& te::da::core::Index::operator=(const Index& rhs)
{
  if(this != &rhs)
  {
    m_name = rhs.m_name;
    m_properties = rhs.m_properties;
    m_dt = 0;
    m_type = rhs.m_type;
  }

  return *this;
}

