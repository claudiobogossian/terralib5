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
  \file terralib/edt/GeometryManager.cpp

  \brief This is a singleton for managing edited geometries.
*/

// TerraLib
#include "../common/STLUtils.h"
#include "../common/Translator.h"
#include "IdentifiedGeometries.h"
#include "GeometryManager.h"

// Boost
#include <boost/format.hpp>

// STL
#include <cassert>

void te::edit::GeometryManager::addEditedGeometry(const std::string& source, te::da::ObjectId* id, te::gm::Geometry* geom)
{
  IdentifiedGeometries* igeom = getIdentifiedGeometries(source);
  if(igeom == 0)
  {
    igeom = new IdentifiedGeometries(source);
    igeom->add(id, geom);
    m_editedGeometries[source] = igeom;
  }
  else
    igeom->add(id, geom);
}

te::edit::IdentifiedGeometries* te::edit::GeometryManager::getIdentifiedGeometries(const std::string& source) const
{
  std::map<std::string, IdentifiedGeometries*>::const_iterator it = m_editedGeometries.find(source);

  if(it == m_editedGeometries.end())
    return 0;
  
  return it->second;
}

std::vector<te::edit::IdGeom*> te::edit::GeometryManager::getGeometries(const std::string& source, const te::gm::Envelope& e, int srid) const
{
  IdentifiedGeometries* igeom = getIdentifiedGeometries(source);

  if(igeom == 0)
    return std::vector<te::edit::IdGeom*>();

  return igeom->getGeometries(e, srid);
}

te::edit::IdGeom* te::edit::GeometryManager::getGeometry(const std::string& source, const te::gm::Envelope& e, int srid) const
{
  IdentifiedGeometries* igeom = getIdentifiedGeometries(source);

  if(igeom == 0)
    return 0;

  return igeom->getGeometry(e, srid);
}

void te::edit::GeometryManager::clearAll()
{
  clearEditedGeometries();
}

void te::edit::GeometryManager::clearEditedGeometries()
{
  te::common::FreeContents(m_editedGeometries);
  m_editedGeometries.clear();
}

te::edit::GeometryManager::GeometryManager()
{
}

te::edit::GeometryManager::~GeometryManager()
{
  clearAll();
}

