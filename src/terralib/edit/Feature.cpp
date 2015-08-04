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
  \file terralib/edit/Feature.h

  \brief This class represents a geographic feature.
*/

// TerraLib
#include "../common/STLUtils.h"
#include "../dataaccess/dataset/ObjectId.h"
#include "../datatype/AbstractData.h"
#include "../geometry/Geometry.h"
#include "Feature.h"
#include "Utils.h"

// STL
#include <cassert>

te::edit::Feature::Feature()
  : m_geom(0)
{
  m_id = GenerateId();
}

te::edit::Feature::Feature(te::da::ObjectId* id)
  : m_id(id),
    m_geom(0)
{
  assert(m_id);
}

te::edit::Feature::Feature(te::da::ObjectId* id, te::gm::Geometry* geom)
  : m_id(id),
    m_geom(geom)
{
  assert(m_id);
  assert(m_geom);
}

te::edit::Feature::~Feature()
{
  delete m_id;
  m_geom = 0;
  delete m_geom;
  te::common::FreeContents(m_data);
}

void te::edit::Feature::set(te::da::ObjectId* id, te::gm::Geometry* geom)
{
  setId(id);
  setGeometry(geom);
}

void te::edit::Feature::setId(te::da::ObjectId* id)
{
  assert(id);

  delete m_id;
  m_id = id;
}

void te::edit::Feature::setGeometry(te::gm::Geometry* geom)
{
  assert(geom);

  delete m_geom;
  m_geom = geom;
}

void te::edit::Feature::setData(const std::map<std::size_t, te::dt::AbstractData*>& data)
{
  te::common::FreeContents(m_data);
  m_data = data;
}

te::da::ObjectId* te::edit::Feature::getId() const
{
  return m_id;
}

te::gm::Geometry* te::edit::Feature::getGeometry() const
{
  return m_geom;
}

const std::map<std::size_t, te::dt::AbstractData*>& te::edit::Feature::getData() const
{
  return m_data;
}

bool te::edit::Feature::isEquals(te::da::ObjectId* id)
{
  assert(m_id);
  assert(id);

  return m_id->getValueAsString() == id->getValueAsString();
}

te::edit::Feature* te::edit::Feature::clone() const
{
  assert(m_id);

  // The clone
  Feature* f = new Feature(m_id->clone());

  // Clone geometry
  if(m_geom)
    f->setGeometry(dynamic_cast<te::gm::Geometry*>(m_geom->clone()));

  // Clone data
  std::map<std::size_t, te::dt::AbstractData*> data;
  for(std::map<std::size_t, te::dt::AbstractData*>::const_iterator it = m_data.begin(); it != m_data.end(); ++it)
  {
    assert(it->second);
    data[it->first] = it->second->clone();
  }

  f->setData(data);

  return f;
}
