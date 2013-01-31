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
#include "../common/Translator.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/datasource/DataSourceCatalogLoader.h"
#include "../datatype/Enums.h"
#include "../datatype/Property.h"
#include "../geometry/Enums.h"
#include "../geometry/Envelope.h"
#include "../raster/RasterProperty.h"
#include "../raster/Grid.h"
#include "DataSet.h"
#include "DataSource.h"
#include "DataSourceTransactor.h"
#include "Raster.h"
#include "Utils.h"

// STL
#include <cassert>
#include <memory>

// Boost
#include <boost/filesystem.hpp>

te::gdal::DataSet::DataSet(DataSourceTransactor* t, te::da::DataSetType* dt, te::common::AccessPolicy rwRole)
  : m_transactor(t),
    m_dsType(dt),
    m_rwRole(rwRole),
    m_size(1),
    m_i(-1)
{
}

void te::gdal::DataSet::loadTypeInfo()
{
  if(m_dsType->isFullLoaded())
    return;

  std::auto_ptr<te::da::DataSourceCatalogLoader> cloader(m_transactor->getCatalogLoader());

  te::da::DataSetType* dt = cloader->getDataSetType(m_dsType->getName(), true);

  delete m_dsType;

  m_dsType = dt; 
}

te::gm::Envelope* te::gdal::DataSet::getExtent(const te::dt::Property* p) 
{
  assert(p && (p->getType() == te::dt::RASTER_TYPE));

  if(!m_dsType->isFullLoaded())
    loadTypeInfo();

  const te::rst::RasterProperty* rp = static_cast<const te::rst::RasterProperty*>(m_dsType->getProperty(0));

  return new te::gm::Envelope(*(rp->getGrid()->getExtent()));
}

te::rst::Raster* te::gdal::DataSet::getRaster(int i) const
{
  assert(i <static_cast<int>(m_dsType->getProperties().size()));
  assert(m_dsType->getProperty(i)->getType() == te::dt::RASTER_TYPE);

  te::rst::RasterProperty* rp = static_cast<te::rst::RasterProperty*>(m_dsType->getProperty(i));

  te::gdal::Raster* rs = 0;

  rs = new te::gdal::Raster(rp->getInfo().at("URI"), m_rwRole);

  return rs;
}

te::rst::Raster* te::gdal::DataSet::getRaster(const std::string& name) const
{
  int idx = static_cast<int>(m_dsType->getPropertyPosition(name));

  return getRaster(idx);
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

bool te::gdal::DataSet::moveFirst()
{
  m_i = 0; 
  return (m_size != 0); 
}

bool te::gdal::DataSet::moveBeforeFirst() 
{
  m_i = -1;
  return (m_size != 0);
}

bool te::gdal::DataSet::moveLast() 
{
  m_i = m_size - 1;
  return (m_size != 0);
}

bool te::gdal::DataSet::moveAfterLast()
{
  m_i = m_size;
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
