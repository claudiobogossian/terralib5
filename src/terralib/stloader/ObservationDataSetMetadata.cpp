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
  \file ObservationDataSetMetadata.cpp

  \brief This file contains a class called ObservationDataSetMetadata
*/

//ST
#include "ObservationDataSetMetadata.h"

te::stloader::ObservationDataSetMetadata::ObservationDataSetMetadata(ObservationSetType type)
  : m_type(type)
{  
}

te::stloader::ObservationDataSetMetadata::ObservationDataSetMetadata(ObservationSetType type, 
                               const std::vector<int>& obsIdxs,
                               const std::vector<int>& phTimeIdxs)
  : m_type(type),
    m_observedPropIdxs(obsIdxs),
    m_phTimePropIdxs(phTimeIdxs)
{  
}

te::stloader::ObservationDataSetMetadata::ObservationDataSetMetadata(ObservationSetType type, 
                               const std::vector<int>& obsIdxs,
                               int phTimeIdx)
  : m_type(type),
    m_observedPropIdxs(obsIdxs)
{  
  m_phTimePropIdxs.push_back(phTimeIdx);
}

te::stloader::ObservationDataSetMetadata::ObservationDataSetMetadata(ObservationSetType type, const std::vector<int>& obsIdxs,
                               const std::vector<int>& phTimeIdxs, const std::vector<int>& valTimeIdx, 
                               int resTimeIdx)
   :  m_type(type),
      m_observedPropIdxs(obsIdxs),
      m_phTimePropIdxs(phTimeIdxs),
      m_validTimePropIdxs(valTimeIdx),
      m_resultTimePropIdx(resTimeIdx)
{  
}

te::stloader::ObservationDataSetMetadata::ObservationSetType te::stloader::ObservationDataSetMetadata::getType() const
{
  return m_type;
}

void te::stloader::ObservationDataSetMetadata::setType(ObservationSetType type)
{
  m_type = type;
}      

std::vector<int>& te::stloader::ObservationDataSetMetadata::getObservedPropertiesIdx() 
{
  return m_observedPropIdxs;
}

void te::stloader::ObservationDataSetMetadata::setObservedPropertiesIdx(const std::vector<int>& idxs)
{
  m_observedPropIdxs = idxs;
}

std::vector<int>& te::stloader::ObservationDataSetMetadata::getPhenomenonTimeIdx() 
{
  return m_phTimePropIdxs;
}

void te::stloader::ObservationDataSetMetadata::setPhenomenonTimeIdx(const std::vector<int>& idxs)
{
  m_phTimePropIdxs = idxs;
}

void te::stloader::ObservationDataSetMetadata::setPhenomenonTimeIdx(int idx)
{
  m_phTimePropIdxs.push_back(idx);
}

std::vector<int>& te::stloader::ObservationDataSetMetadata::getValidTimeIdx() 
{
  return m_validTimePropIdxs;
}

void te::stloader::ObservationDataSetMetadata::setValidTimeIdx(const std::vector<int>& idxs)
{
  m_validTimePropIdxs = idxs;
}

int te::stloader::ObservationDataSetMetadata::getResultTimeIdx() const
{
  return m_resultTimePropIdx;
}

void te::stloader::ObservationDataSetMetadata::setValidTimeIdx(int idx)
{
  m_resultTimePropIdx = idx;
} 

