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
#include "../common/STLUtils.h"
#include "../common/Translator.h"
#include "../dataaccess/dataset/DataSet.h"
#include "Snap.h"
#include "SnapManager.h"

// Boost
#include <boost/format.hpp>

// STL
#include <cassert>

void te::edit::SnapManager::buildSnap(const std::string& source, int srid, te::da::DataSet* dataset)
{
  Snap* snap = getSnap(source);

  if(snap == 0)
  {
    // Not found! Create a new snap associated with the given source
    snap = new Snap(source, srid);

    // Build the snap
    snap->build(dataset);

    // Store!
    m_snaps[source] = snap;
  }
  else
    snap->build(dataset);
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

void te::edit::SnapManager::clearAll()
{
  te::common::FreeContents(m_snaps);
  m_snaps.clear();
}

te::edit::SnapManager::SnapManager()
{
}

te::edit::SnapManager::~SnapManager()
{
}
