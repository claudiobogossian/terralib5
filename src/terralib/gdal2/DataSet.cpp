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
  \file terralib/gdal/DataSet.cpp

  \brief Implementation of a DataSet for GDAL data provider.  
*/

// TerraLib
#include "../dataaccess2/dataset/DataSetType.h"
#include "../datatype/Enums.h"
#include "../geometry/Envelope.h"
#include "../raster/RasterProperty.h"
#include "../raster/Grid.h"
#include "DataSet.h"
#include "Raster.h"

// STL
#include <cassert>
#include <memory>

te::gdal::DataSet::DataSet(te::da::DataSetType* dt)
  : m_dsType(dt),
    m_rwRole(te::common::RAccess),
    m_size(1),
    m_i(-1)
{
  assert(m_dsType);

//  if(m_dsType == 0)
//  {
//    std::auto_ptr<te::da::DataSourceCatalogLoader> cloader(m_transactor->getCatalogLoader());
//
//    te::da::DataSetType* dt = cloader->getDataSetType(m_dsType->getName(), true);
//
//    assert(dt);
//
//    m_dsType = dt;
//  }
}

te::gdal::DataSet::~DataSet()
{
  delete m_dsType;
}

te::gm::Envelope* te::gdal::DataSet::getExtent(std::size_t i) 
{
  assert(getPropertyDataType(i) == te::dt::RASTER_TYPE);

  const te::rst::RasterProperty* rp = static_cast<const te::rst::RasterProperty*>(m_dsType->getProperty(i));

  return new te::gm::Envelope(*(rp->getGrid()->getExtent()));
}

std::size_t te::gdal::DataSet::getNumProperties() const
{
  return m_dsType->size();
}

int te::gdal::DataSet::getPropertyDataType(std::size_t pos) const
{
  return m_dsType->getProperty(pos)->getType();
}

std::string te::gdal::DataSet::getPropertyName(std::size_t pos) const
{
  return m_dsType->getProperty(pos)->getName();
}

std::string te::gdal::DataSet::getDatasetNameOfProperty(std::size_t pos) const
{
  return "";
}

te::rst::Raster* te::gdal::DataSet::getRaster(std::size_t i) const
{
  assert(i < getNumProperties());
  assert(getPropertyDataType(i) == te::dt::RASTER_TYPE);

  te::rst::RasterProperty* rp = static_cast<te::rst::RasterProperty*>(m_dsType->getProperty(i));

  te::gdal::Raster* rs = new te::gdal::Raster(rp->getInfo().at("URI"), m_rwRole);

  return rs;
}

te::rst::Raster* te::gdal::DataSet::getRaster(const std::string& name) const
{
  std::size_t pos = m_dsType->getPropertyPosition(name);

  return getRaster(pos);
}

bool te::gdal::DataSet::moveNext()
{
  ++m_i;
  return (m_i < m_size);
}

bool te::gdal::DataSet::movePrevious()
{
  --m_i;
  return (m_i > -1);
}

bool te::gdal::DataSet::moveBeforeFirst() 
{
  m_i = -1;
  return (m_size != 0);
}

bool te::gdal::DataSet::moveFirst()
{
  m_i = 0; 
  return (m_size != 0);
}

bool te::gdal::DataSet::moveLast()
{
  m_i = m_size - 1;
  return (m_size != 0);
}

bool te::gdal::DataSet::move(std::size_t i)
{
  m_i = static_cast<int>(i);
  return (m_i < m_size);
}

bool te::gdal::DataSet::isAtBegin() const
{
  return (m_i == 0);
}

bool te::gdal::DataSet::isBeforeBegin() const
{
  return (m_i < 0);
}

bool te::gdal::DataSet::isAtEnd() const
{
  return (m_i == (m_size - 1));
}

bool te::gdal::DataSet::isAfterEnd() const
{
  return (m_i >= m_size);
}

