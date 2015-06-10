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
  \file terralib/memory/RasterFactory.cpp

  \brief This is the concrete factory for In-Memory driver.
*/

// TerraLib
#include "../common/StringUtils.h"
#include "../common/Translator.h"
#include "Exception.h"
#include "Raster.h"
#include "RasterFactory.h"

// STL
#include <memory>

te::mem::RasterFactory te::mem::RasterFactory::sm_factory;

static std::string sg_rasterFactoryId(TE_MEMORY_DRIVER_IDENTIFIER);

te::mem::RasterFactory::~RasterFactory()
{
}

const std::string& te::mem::RasterFactory::getType() const
{
  return sg_rasterFactoryId;
}

void te::mem::RasterFactory::getCreationalParameters(std::vector< std::pair<std::string, std::string> >& params) const
{
  params.push_back(std::pair<std::string, std::string>("MEM_SRC_RASTER_DRIVER_TYPE", ""));
  params.push_back(std::pair<std::string, std::string>("MEM_TILED_RASTER", ""));
  params.push_back(std::pair<std::string, std::string>("MEM_RASTER_TILE_WIDTH", ""));
  params.push_back(std::pair<std::string, std::string>("MEM_RASTER_TILE_HEIGHT", ""));

  params.push_back(std::pair<std::string, std::string>("MEM_IS_DATA_BUFFER", ""));
  params.push_back(std::pair<std::string, std::string>("MEM_BUFFER_NROWS", ""));
  params.push_back(std::pair<std::string, std::string>("MEM_BUFFER_NCOLS", ""));
  params.push_back(std::pair<std::string, std::string>("MEM_BUFFER_DATATYPE", ""));
  params.push_back(std::pair<std::string, std::string>("MEM_BUFFER_NBANDS", ""));
  params.push_back(std::pair<std::string, std::string>("MEM_BUFFER_SRID", ""));
  params.push_back(std::pair<std::string, std::string>("MEM_BUFFER_RES_X", ""));
  params.push_back(std::pair<std::string, std::string>("MEM_BUFFER_RES_Y", ""));
  params.push_back(std::pair<std::string, std::string>("MEM_BUFFER_MIN_X", ""));
  params.push_back(std::pair<std::string, std::string>("MEM_BUFFER_MIN_Y", ""));
  params.push_back(std::pair<std::string, std::string>("MEM_BUFFER_MAX_X", ""));
  params.push_back(std::pair<std::string, std::string>("MEM_BUFFER_MAX_Y", ""));

  params.push_back(std::pair<std::string, std::string>("MEM_RASTER_NROWS", ""));
  params.push_back(std::pair<std::string, std::string>("MEM_RASTER_NCOLS", ""));
  params.push_back(std::pair<std::string, std::string>("MEM_RASTER_DATATYPE", ""));
  params.push_back(std::pair<std::string, std::string>("MEM_RASTER_NBANDS", ""));
  params.push_back(std::pair<std::string, std::string>("MEM_RASTER_SRID", ""));
  params.push_back(std::pair<std::string, std::string>("MEM_RASTER_RES_X", ""));
  params.push_back(std::pair<std::string, std::string>("MEM_RASTER_RES_Y", ""));
  params.push_back(std::pair<std::string, std::string>("MEM_RASTER_MIN_X", ""));
  params.push_back(std::pair<std::string, std::string>("MEM_RASTER_MIN_Y", ""));
  params.push_back(std::pair<std::string, std::string>("MEM_RASTER_MAX_X", ""));
  params.push_back(std::pair<std::string, std::string>("MEM_RASTER_MAX_Y", ""));
}

te::mem::RasterFactory::RasterFactory()
  : te::rst::RasterFactory(TE_MEMORY_DRIVER_IDENTIFIER)
{
}

te::rst::Raster* te::mem::RasterFactory::create(te::rst::Grid* g,
                                                const std::vector<te::rst::BandProperty*> bands,
                                                const std::map<std::string, std::string>& rinfo,
                                                void* h,
                                                void (*deleter)(void*))
{
  std::auto_ptr<Raster> raster(new Raster);

  raster->create(g, bands, rinfo, h, deleter);

  return raster.release();
}

te::rst::Raster* te::mem::RasterFactory::build()
{
  return new Raster;
}

std::map<std::string, std::string> te::mem::RasterFactory::getCapabilities() const
{
  std::map<std::string, std::string> capabilities = std::map<std::string, std::string>();

  return capabilities;
}
