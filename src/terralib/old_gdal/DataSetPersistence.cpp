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
  \file terralib/gdal/DataSetPersistence.cpp

  \brief This class is responsible for persisting a dataset using the GDAL driver.
*/

// TerraLib
#include "../common/Translator.h"
#include "../dataaccess/dataset/DataSetType.h"
#include "../raster/Raster.h"
#include "../raster/RasterProperty.h"
#include "DataSet.h"
#include "DataSetPersistence.h"
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

void te::gdal::DataSetPersistence::remove(const std::string& /*datasetName*/, const te::da::ObjectIdSet* /*oids*/)
{
  throw te::common::Exception(TR_GDAL("GDAL driver does not support this method."));
}

void te::gdal::DataSetPersistence::add(const std::string& /*datasetName*/,
                                       te::da::DataSet* /*d*/,
                                       const std::map<std::string, std::string>& /*options*/,
                                       std::size_t /*limit*/)
{
  throw te::common::Exception(TR_GDAL("GDAL driver: not implemented yet."));
}

void te::gdal::DataSetPersistence::update(const std::string& /*datasetName*/,
                                          te::da::DataSet* /*dataset*/,
                                          const std::vector<std::size_t>& /*properties*/,
                                          const te::da::ObjectIdSet* /*oids*/,
                                          const std::map<std::string, std::string>& /*options*/,
                                          std::size_t /*limit*/)
{
// The raster interface should be used.
  throw te::common::Exception(TR_GDAL("GDAL driver does not support this method."));
}
