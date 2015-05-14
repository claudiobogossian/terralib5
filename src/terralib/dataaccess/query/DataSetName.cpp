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
  \file terralib/dataaccess/query/DataSetName.cpp

  \brief A class that models the name of a dataset used in a From clause.
*/

// TerraLib
#include "DataSetName.h"

te::da::DataSetName::DataSetName(const std::string& name, const std::string& alias)
  : FromItem(alias),
    m_name(name)
{
}

te::da::DataSetName::DataSetName(const DataSetName& rhs)
  : FromItem(rhs),
    m_name(rhs.m_name)
{  
}

te::da::DataSetName& te::da::DataSetName::operator=(const DataSetName& rhs)
{
  if(this != &rhs)
  {
    FromItem::operator=(rhs);
    m_name = rhs.m_name;
  }

  return *this;
}

te::da::FromItem* te::da::DataSetName::clone() const
{
  return new DataSetName(*this);
}

const std::string& te::da::DataSetName::getName() const
{
  return m_name;
}

void te::da::DataSetName::setName(const std::string& name)
{
  m_name = name;
}

