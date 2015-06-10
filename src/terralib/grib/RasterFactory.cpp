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
  \file terralib/grib/RasterFactory.cpp

  \brief This is the concrete factory for GRIB.
*/

// TerraLib
#include "../common/StringUtils.h"
#include "../common/Translator.h"
#include "Exception.h"
#include "RasterFactory.h"
#include "Raster.h"

// STL
#include <memory>

te::grib::RasterFactory* te::grib::RasterFactory::sm_factory(0);

te::grib::RasterFactory::~RasterFactory()
{
}

const std::string& te::grib::RasterFactory::getType() const
{
  throw Exception(TR_GRIB("Not implemented yet!"));
}

void te::grib::RasterFactory::getCreationalParameters(std::vector< std::pair<std::string, std::string> >& params) const
{
  // see terralib/memory/RasterFactory.cpp (getCreationalParameters)
  throw Exception(TR_GRIB("Not implemented yet!"));
}

void te::grib::RasterFactory::initialize()
{
  finalize();
  sm_factory = new RasterFactory;
}

void te::grib::RasterFactory::finalize()
{
  delete sm_factory;
  sm_factory = 0;
}

te::grib::RasterFactory::RasterFactory()
  : te::rst::RasterFactory(GRIB_DRIVER_IDENTIFIER)
{
}

te::rst::Raster* te::grib::RasterFactory::create(te::rst::Grid* g,
                                                 const std::vector<te::rst::BandProperty*> bands,
                                                 const std::map<std::string, std::string>& rinfo,
                                                 void* h, void (*deleter)(void*))
{
  std::auto_ptr<Raster> raster(new Raster);

  raster->create(g, bands, rinfo, h, deleter);

  return raster.release();
}

te::rst::Raster* te::grib::RasterFactory::build()
{
  return new Raster;
}

std::map<std::string, std::string> te::grib::RasterFactory::getCapabilities() const
{
  std::map<std::string, std::string> capabilities;

  capabilities["supported_formats"] = "grib";

  return capabilities;
}
