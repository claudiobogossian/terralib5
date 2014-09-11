/*  Copyright (C) 2008-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/edit/IdGeometry.h

  \brief This class represents an identified geometry.
*/

// TerraLib
#include "../dataaccess/dataset/ObjectId.h"
#include "../geometry/Geometry.h"
#include "IdGeometry.h"

// STL
#include <cassert>

te::edit::IdGeometry::IdGeometry(te::da::ObjectId* id, te::gm::Geometry* geom)
  : m_id(id),
    m_geom(geom)
{
  assert(m_id);
  assert(m_geom);
}

te::edit::IdGeometry::~IdGeometry()
{
  delete m_id;
  delete m_geom;
}

void te::edit::IdGeometry::set(te::da::ObjectId* id, te::gm::Geometry* geom)
{
  setId(id);
  setGeometry(geom);
}

void te::edit::IdGeometry::setId(te::da::ObjectId* id)
{
  assert(id);
  delete m_id;
  m_id = id;
}

void te::edit::IdGeometry::setGeometry(te::gm::Geometry* geom)
{
  assert(geom);
  delete m_geom;
  m_geom = geom;
}

te::da::ObjectId* te::edit::IdGeometry::getId() const
{
  return m_id;
}

te::gm::Geometry* te::edit::IdGeometry::getGeometry() const
{
  return m_geom;
}

bool te::edit::IdGeometry::isEquals(te::da::ObjectId* id)
{
  assert(m_id);
  assert(id);

  return m_id->getValueAsString() == id->getValueAsString();
}

te::edit::IdGeometry* te::edit::IdGeometry::clone() const
{
  return new IdGeometry(m_id->clone(), dynamic_cast<te::gm::Geometry*>(m_geom->clone()));
}
