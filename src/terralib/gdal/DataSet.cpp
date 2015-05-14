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
  \file terralib/gdal/DataSet.cpp

  \brief Implementation of a DataSet for GDAL data provider.  
*/

// TerraLib
#include "../dataaccess/dataset/DataSetType.h"
#include "../datatype/Enums.h"
#include "../geometry/Envelope.h"
#include "../raster/RasterProperty.h"
#include "../raster/Grid.h"
#include "DataSet.h"
#include "Raster.h"

// STL
#include <cassert>
#include <memory>

te::gdal::DataSet::DataSet(std::auto_ptr<te::da::DataSetType> dt,
  te::common::AccessPolicy rwRole, std::string uri)
  : m_dsType(dt),
    m_uri(uri),
    m_rwRole(rwRole),
    m_size(1),
    m_i(-1)
{}

te::gdal::DataSet::~DataSet()
{}

void te::gdal::DataSet::setURI(const std::string& uri)
{
  m_uri = uri;
}

std::auto_ptr<te::gm::Envelope> te::gdal::DataSet::getExtent(std::size_t i) 
{
  assert(getPropertyDataType(i) == te::dt::RASTER_TYPE);
  
  const te::rst::RasterProperty* rp = static_cast<const te::rst::RasterProperty*>(m_dsType->getProperty(i));
  
  const te::gm::Envelope* env = rp->getGrid()->getExtent();

  return std::auto_ptr<te::gm::Envelope>(new te::gm::Envelope(env->getLowerLeftX(), env->getLowerLeftY(),
                                                              env->getUpperRightX(), env->getUpperRightY()));
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

std::auto_ptr<te::rst::Raster> te::gdal::DataSet::getRaster(std::size_t i) const
{
  assert(i < getNumProperties());
  assert(getPropertyDataType(i) == te::dt::RASTER_TYPE);

  te::gdal::Raster* rs = new te::gdal::Raster(m_uri, m_rwRole);

  return std::auto_ptr<te::rst::Raster>(rs);
}

std::auto_ptr<te::rst::Raster> te::gdal::DataSet::getRaster(const std::string& name) const
{
  std::size_t pos = m_dsType->getPropertyPosition(name);

  return std::auto_ptr<te::rst::Raster>(getRaster(pos));
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

