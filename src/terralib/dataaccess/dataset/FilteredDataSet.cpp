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
  \file terralib/dataaccess/dataset/FilteredDataSet.cpp

  \brief This class represents a filtered data set.
*/

// TerraLib
#include "../../geometry/Envelope.h"
#include "FilteredDataSet.h"

// STL
#include <cassert>

te::da::FilteredDataSet::FilteredDataSet(DataSet* dataset, const std::vector<std::size_t>& positions, bool isOwner)
  : m_ds(dataset, isOwner),
    m_positions(positions),
    m_i(-1)
{
  assert(dataset);

  std::sort(m_positions.begin(), m_positions.end());
}

te::da::FilteredDataSet::FilteredDataSet(DataSet* dataset, const DataSetCapabilities& capabilities,
                                         const std::vector<std::size_t>& positions, bool isOwner)
  : m_ds(dataset, isOwner),
    m_capabilites(capabilities),
    m_positions(positions),
    m_i(-1)
{
  assert(dataset);

  std::sort(m_positions.begin(), m_positions.end());
}

te::da::FilteredDataSet::~FilteredDataSet()
{
}

te::common::TraverseType te::da::FilteredDataSet::getTraverseType() const
{
  return m_ds->getTraverseType();
}

te::common::AccessPolicy te::da::FilteredDataSet::getAccessPolicy() const
{
  return m_ds->getAccessPolicy();
}

std::auto_ptr<te::gm::Envelope> te::da::FilteredDataSet::getExtent(std::size_t i)
{
  return m_ds->getExtent(i);
}

std::size_t te::da::FilteredDataSet::getNumProperties() const
{
  return m_ds->getNumProperties();
}

int te::da::FilteredDataSet::getPropertyDataType(std::size_t pos) const
{
  return m_ds->getPropertyDataType(pos);
}

std::string te::da::FilteredDataSet::getPropertyName(std::size_t pos) const
{
  return m_ds->getPropertyName(pos);
}

std::string te::da::FilteredDataSet::getDatasetNameOfProperty(std::size_t pos) const
{
  return m_ds->getDatasetNameOfProperty(pos);
}

bool te::da::FilteredDataSet::isEmpty() const
{
  return m_positions.empty();
}

bool te::da::FilteredDataSet::isConnected() const
{
  return m_ds->isConnected();
}

std::size_t te::da::FilteredDataSet::size() const
{
  return m_positions.size();
}

bool te::da::FilteredDataSet::moveNext()
{
  int oldPos = m_i;

  m_i++;

  if(m_i == size())
    return false;

  if(m_capabilites.supportsEfficientMove() || oldPos == -1)
    return move(static_cast<std::size_t>(m_i));

  assert(m_positions[oldPos] < m_positions[m_i]);

  // Read forward and skip features by the difference of the new and old positions
  for(std::size_t i = m_positions[oldPos]; i < m_positions[m_i]; ++i)
  {
    if(!m_ds->moveNext())
      return false;
  }

  return true;
}

bool te::da::FilteredDataSet::movePrevious()
{
  m_i--;

  if(m_i < 0)
    return false;

  return move(static_cast<std::size_t>(m_i));
}

bool te::da::FilteredDataSet::moveBeforeFirst()
{
  m_i = -1;
  return true;
}

bool te::da::FilteredDataSet::moveFirst()
{
  m_i = 0;
  return move(m_i);
}

bool te::da::FilteredDataSet::moveLast()
{
  m_i = size() - 1;
  return move(m_i);
}

bool te::da::FilteredDataSet::move(std::size_t i)
{
  assert(i < size());

  return m_ds->move(m_positions[i]);
}

bool te::da::FilteredDataSet::isAtBegin() const
{
  return m_i == 0;
}

bool te::da::FilteredDataSet::isBeforeBegin() const
{
  return m_i == -1;
}

bool te::da::FilteredDataSet::isAtEnd() const
{
  return m_i == size() - 1;
}

bool te::da::FilteredDataSet::isAfterEnd() const
{
  return m_i == size();
}

char te::da::FilteredDataSet::getChar(std::size_t i) const
{
  return m_ds->getChar(i);
}

unsigned char te::da::FilteredDataSet::getUChar(std::size_t i) const
{
  return m_ds->getUChar(i);
}

boost::int16_t te::da::FilteredDataSet::getInt16(std::size_t i) const
{
  return m_ds->getInt16(i);
}

boost::int32_t te::da::FilteredDataSet::getInt32(std::size_t i) const
{
  return m_ds->getInt32(i);
}

boost::int64_t te::da::FilteredDataSet::getInt64(std::size_t i) const
{
  return m_ds->getInt64(i);
}

bool te::da::FilteredDataSet::getBool(std::size_t i) const
{
  return m_ds->getBool(i);
}

float te::da::FilteredDataSet::getFloat(std::size_t i) const
{
  return m_ds->getFloat(i);
}

double te::da::FilteredDataSet::getDouble(std::size_t i) const
{
  return m_ds->getDouble(i);
}

std::string te::da::FilteredDataSet::getNumeric(std::size_t i) const
{
  return m_ds->getNumeric(i);
}

std::string te::da::FilteredDataSet::getString(std::size_t i) const
{
  return m_ds->getString(i);
}

std::auto_ptr<te::dt::ByteArray> te::da::FilteredDataSet::getByteArray(std::size_t i) const
{
  return m_ds->getByteArray(i);
}

std::auto_ptr<te::gm::Geometry> te::da::FilteredDataSet::getGeometry(std::size_t i) const
{
  return m_ds->getGeometry(i);
}

std::auto_ptr<te::rst::Raster> te::da::FilteredDataSet::getRaster(std::size_t i) const
{
  return m_ds->getRaster(i);
}

std::auto_ptr<te::dt::DateTime> te::da::FilteredDataSet::getDateTime(std::size_t i) const
{
  return m_ds->getDateTime(i);
}

std::auto_ptr<te::dt::Array> te::da::FilteredDataSet::getArray(std::size_t i) const
{
  return m_ds->getArray(i);
}

bool te::da::FilteredDataSet::isNull(std::size_t i) const
{
  return m_ds->isNull(i);
}
