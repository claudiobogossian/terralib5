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
  \file terralib/dataaccess/core/dataset/ForeignKey.cpp

  \brief It models a foreign key constraint for a DataSetType.
*/

// TerraLib
#include "ForeignKey.h"

te::da::core::ForeignKey::ForeignKey(const std::string& name)
  : Constraint(name),
    m_onDelete(NO_ACTION),
    m_onUpdate(NO_ACTION)
{
}

te::da::core::ForeignKey::ForeignKey(const ForeignKey& rhs)
  : Constraint(rhs),
    m_refDataset(rhs.m_refDataset),
    m_properties(rhs.m_properties),
    m_refProperties(rhs.m_refProperties),
    m_onDelete(rhs.m_onDelete),
    m_onUpdate(rhs.m_onUpdate)
{
}

te::da::core::ForeignKey& te::da::core::ForeignKey::operator=(const ForeignKey& rhs)
{
  if(this != &rhs)
  {
    Constraint::operator=(rhs);

    m_refDataset = rhs.m_refDataset;

    m_properties = rhs.m_properties;

    m_refProperties = rhs.m_refProperties;

    m_onDelete = rhs.m_onDelete;

    m_onUpdate = rhs.m_onUpdate;
  }

  return *this;
}

te::da::core::Constraint* te::da::core::ForeignKey::clone()
{
  return new ForeignKey(*this);
}
