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
  \file LocatedTimeSeriesDataSetMetadata.cpp

  \brief This file contains a class called LocatedTimeSeriesDataSetMetadata
*/

//STLoader
#include "LocatedTimeSeriesDataSetMetadata.h"

te::stloader::LocatedTimeSeriesDataSetMetadata::LocatedTimeSeriesDataSetMetadata()
  : ObservationDataSetMetadata(te::stloader::ObservationDataSetMetadata::LOCATEDTIMESERIES),
    m_sLocPropIdx(-1),
    m_idPropIdx(-1)
{
}

te::stloader::LocatedTimeSeriesDataSetMetadata::LocatedTimeSeriesDataSetMetadata(int sLocPropIdx, int idPropIdx)
  : ObservationDataSetMetadata(te::stloader::ObservationDataSetMetadata::LOCATEDTIMESERIES),
    m_sLocPropIdx(sLocPropIdx),
    m_idPropIdx(idPropIdx)
{
}

int te::stloader::LocatedTimeSeriesDataSetMetadata::getSpatialLocationPropertyIdx() const
{
  return m_sLocPropIdx;
}

void te::stloader::LocatedTimeSeriesDataSetMetadata::setSpatialLocationPropertyIdx(int idx)
{
  m_sLocPropIdx = idx;
}

int te::stloader::LocatedTimeSeriesDataSetMetadata::getIdPropertyIdx() const
{
  return m_idPropIdx;
}

void te::stloader::LocatedTimeSeriesDataSetMetadata::setIdPropertyIdx(int idx)
{
  m_idPropIdx = idx;
}    




