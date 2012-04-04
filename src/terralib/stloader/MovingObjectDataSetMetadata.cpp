/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file MovingObjectDataSetMetadata.cpp

  \brief This file contains a class called MovingObjectDataSetMetadata
*/

//stloader
#include "MovingObjectDataSetMetadata.h"

te::stloader::MovingObjectDataSetMetadata::MovingObjectDataSetMetadata()
  : ObservationDataSetMetadata(te::stloader::ObservationDataSetMetadata::MOVINGOBJECT),
    m_geomPropIdx(-1),
    m_idPropIdx(-1)
{
}

te::stloader::MovingObjectDataSetMetadata::MovingObjectDataSetMetadata(int phTimePropIdx, int geomPropIdx, int idPropIdx)
  : ObservationDataSetMetadata(te::stloader::ObservationDataSetMetadata::MOVINGOBJECT),
    m_geomPropIdx(geomPropIdx),
    m_idPropIdx(idPropIdx)
{
  m_phTimePropIdxs.push_back(phTimePropIdx);
  m_observedPropIdxs.push_back(geomPropIdx);
}

int te::stloader::MovingObjectDataSetMetadata::getGeometryPropertyIdx() const
{
  return m_geomPropIdx;
}

void te::stloader::MovingObjectDataSetMetadata::setGeometryPropertyIdx(int idx)
{
  m_geomPropIdx = idx;
}

int te::stloader::MovingObjectDataSetMetadata::getIdPropertyIdx() const
{
  return m_idPropIdx;
}

void te::stloader::MovingObjectDataSetMetadata::setIdPropertyIdx(int idx)
{
  m_idPropIdx = idx;
}    


