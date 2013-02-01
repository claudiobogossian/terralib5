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
  \file terralib/gdal/DataSetPersistence.cpp

  \brief This class is responsible for persisting a dataset using the GDAL driver.
*/

// TerraLib
#include "../common/Translator.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../dataaccess/datasource/DataSource.h"
#include "../dataaccess/datasource/DataSourceCatalog.h"
#include "../raster/Raster.h"
#include "../raster/RasterProperty.h"
#include "DataSet.h"
#include "DataSetPersistence.h"
#include "DataSetTypePersistence.h"
#include "DataSourceTransactor.h"
#include "Exception.h"
#include "Raster.h"
#include "Utils.h"

// Boost
#include <boost/filesystem.hpp>

// STL
#include <cassert>
#include <complex>

te::gdal::DataSetPersistence::DataSetPersistence(DataSourceTransactor* t)
  : m_t(t)
{
}

te::gdal::DataSetPersistence::~DataSetPersistence()
{
}

void te::gdal::DataSetPersistence::create(te::da::DataSetType* dt, te::da::DataSet* d, const std::map<std::string, std::string>& /*options*/, std::size_t /*limit*/)
{
  te::rst::RasterProperty* rprop = static_cast<te::rst::RasterProperty*>(dt->getProperty(0));

  GDALDataset* gds = CreateRaster(rprop->getGrid(), rprop->getBandProperties(), rprop->getInfo());

  if(!gds)
    throw Exception(TR_GDAL("GDAL driver couldn't persist the raster file."));

  te::rst::Raster* rOut = new Raster(gds, te::common::RWAccess);

  te::rst::Raster* rIn = d->getRaster();

  std::complex<double> val;

  for (unsigned int b = 0; b < rIn->getNumberOfBands(); ++b)
  {
    for (unsigned int l = 0; l < rIn->getNumberOfRows(); ++l)
    {
      for (unsigned int c=0; c < rIn->getNumberOfColumns(); ++c)
      {
        rIn->getValue(c,l,val,b);
        rOut->setValue(c,l,val,b);
      }
    }
  }

  delete rIn;

  delete rOut;
}

void te::gdal::DataSetPersistence::remove(const te::da::DataSetType* /*dt*/)
{
  // remover arquivo ou colocar nodata?
  throw te::common::Exception(TR_GDAL("GDAL driver does not support the concept of data set item."));
}

void te::gdal::DataSetPersistence::remove(const std::string& /*datasetName*/)
{
  throw te::common::Exception(TR_GDAL("Not implemented yet!"));
}

void te::gdal::DataSetPersistence::remove(const te::da::DataSetType* /*dt*/, te::da::DataSet* /*d*/, std::size_t /*limit*/)
{
  throw te::common::Exception(TR_GDAL("GDAL driver does not support the concept of data set item."));
}

void te::gdal::DataSetPersistence::remove(const te::da::DataSetType* /*dt*/, te::da::DataSetItem* /*item*/)
{
  throw te::common::Exception(TR_GDAL("GDAL driver does not support the concept of data set item."));
}

void te::gdal::DataSetPersistence::add(const te::da::DataSetType* /*dt*/, te::da::DataSet* /*d*/, const std::map<std::string, std::string>& /*options*/, std::size_t /*limit*/)
{
  throw te::common::Exception(TR_GDAL("GDAL driver does not support the concept of data set item."));
}

void te::gdal::DataSetPersistence::add(const te::da::DataSetType* /*dt*/, te::da::DataSetItem* /*item*/)
{
  throw te::common::Exception(TR_GDAL("GDAL driver does not support the concept of data set item."));
}

void te::gdal::DataSetPersistence::update(const te::da::DataSetType* /*dt*/,
                                          te::da::DataSet* /*f*/,
                                          const std::vector<te::dt::Property*>& /*properties*/,
                                          const std::map<std::string, std::string>& /*options*/,
                                          std::size_t /*limit*/)
{
  throw te::common::Exception(TR_GDAL("GDAL driver does not support this method."));
  // The raster interface should be used.
}

void te::gdal::DataSetPersistence::update(const te::da::DataSetType* /*dt*/,
                                          te::da::DataSetItem* /*item*/,
                                          const std::vector<te::dt::Property*>& /*properties*/)
{
  throw te::common::Exception(TR_GDAL("GDAL driver does not support this method."));
  // The raster interface should be used.
}

void te::gdal::DataSetPersistence::update(const te::da::DataSetType* /*dt*/,
                                          te::da::DataSet* /*oldD*/,
                                          te::da::DataSet* /*newD*/,
                                          const std::vector<te::dt::Property*>& /*properties*/,
                                          std::size_t /*limit*/)
{
  throw te::common::Exception(TR_GDAL("GDAL driver does not support this method."));
  // The raster interface should be used.
}

void te::gdal::DataSetPersistence::update(const te::da::DataSetType* /*dt*/,
                                          te::da::DataSetItem* /*oldItem*/,
                                          te::da::DataSetItem* /*newItem*/,
                                          const std::vector<te::dt::Property*>& /*properties*/)
{
  throw te::common::Exception(TR_GDAL("GDAL driver does not support this method."));
  // The raster interface should be used.
}

te::da::DataSourceTransactor* te::gdal::DataSetPersistence::getTransactor() const
{
  return m_t;
}
