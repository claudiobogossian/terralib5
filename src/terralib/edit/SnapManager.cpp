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
  \file terralib/edit/SnapManager.cpp

  \brief This is a singleton for managing geometries snap.
*/

// TerraLib
#include "../common/Exception.h"
#include "../common/STLUtils.h"
#include "../common/Translator.h"
#include "../dataaccess/dataset/DataSet.h"
#include "Snap.h"
#include "SnapManager.h"

// Boost
#include <boost/format.hpp>

// STL
#include <cassert>

bool te::edit::SnapManager::hasSnap(const std::string& source) const
{
  return m_snaps.find(source) != m_snaps.end();
}

void te::edit::SnapManager::createSnap(const std::string& source, int srid)
{
  if(hasSnap(source))
    return;

  SnapStrategies::iterator it = m_snapStrategies.find("vertex");

  assert(it != m_snapStrategies.end());

  Snap* snap = it->second(source, srid);

  // Store!
  m_snaps[source] = snap;
}

void te::edit::SnapManager::buildSnap(const std::string& source, int srid, te::da::DataSet* dataset)
{
  Snap* snap = getSnap(source);

  if(snap == 0)
  {
    // Not found! Create a new snap associated with the given source
    SnapStrategies::iterator it = m_snapStrategies.find("vertex");

    assert(it != m_snapStrategies.end());

    snap = it->second(source, srid);

    // Build the snap
    snap->build(dataset);

    // Store!
    m_snaps[source] = snap;
  }
  else
    snap->build(dataset);
}

void te::edit::SnapManager::removeSnap(const std::string& source)
{
  std::map<std::string, Snap*>::iterator it = m_snaps.find(source);

  if(it == m_snaps.end())
    return;

  delete it->second;
  m_snaps.erase(it);
}

const std::map<std::string, te::edit::Snap*>& te::edit::SnapManager::getSnaps() const
{
  return m_snaps;
}

te::edit::Snap* te::edit::SnapManager::getSnap(const std::string& source) const
{
  std::map<std::string, Snap*>::const_iterator it = m_snaps.find(source);

  if(it == m_snaps.end())
    return 0;
  
  return it->second;
}

void te::edit::SnapManager::clear(const std::string& source)
{
  Snap* snap = getSnap(source);

  if(snap)
    snap->clear();
}

void te::edit::SnapManager::clearAll()
{
  te::common::FreeContents(m_snaps);
  m_snaps.clear();
}

bool te::edit::SnapManager::search(const te::gm::Coord2D& coord, int /*srid*/, te::gm::Coord2D& result)
{
  std::map<std::string, Snap*>::const_iterator it;
  for(it = m_snaps.begin(); it != m_snaps.end(); ++it)
  {
    if(it->second->search(coord, result))
      return true;
  }

  return false;
}

void te::edit::SnapManager::setWorld(const double& llx, const double& lly,
                                     const double& urx, const double& ury,
                                     const std::size_t& width, const std::size_t& height)
{
  std::map<std::string, Snap*>::const_iterator it;
  for(it = m_snaps.begin(); it != m_snaps.end(); ++it)
    it->second->setWorld(llx, lly, urx, ury, width, height);
}

void te::edit::SnapManager::reg(const std::string& name, const SnapStrategyFnctType& builder)
{
  SnapStrategies::iterator it = m_snapStrategies.find(name);

  if(it != m_snapStrategies.end())
    throw te::common::Exception((boost::format(TE_TR("There is already a snap strategy registered with the given key %1%.")) % name).str());

  m_snapStrategies[name] = builder;
}

te::edit::SnapManager::SnapManager()
{
}

te::edit::SnapManager::~SnapManager()
{
}
