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
  \file terralib/terralib4/RasterFactory.cpp

  \brief This is the concrete factory for TerraLib 4.x rasters.
*/

// TerraLib
#include "../common/Translator.h"
#include "Config.h"
#include "Exception.h"
#include "Raster.h"
#include "RasterFactory.h"

// STL
#include <memory>

terralib4::RasterFactory* terralib4::RasterFactory::sm_factory(0);

const std::string& terralib4::RasterFactory::getType() const
{
  static std::string driverIdentifier(TERRALIB4_DRIVER_IDENTIFIER);

  return driverIdentifier;
}

void terralib4::RasterFactory::getCreationalParameters(std::vector< std::pair<std::string, std::string> >& /*params*/) const
{
}

void terralib4::RasterFactory::initialize()
{
  finalize();
  sm_factory = new RasterFactory;
}

void terralib4::RasterFactory::finalize()
{
  delete sm_factory;
  sm_factory = 0;
}

terralib4::RasterFactory::RasterFactory()
  : te::rst::RasterFactory(TERRALIB4_DRIVER_IDENTIFIER)
{
}

te::rst::Raster* terralib4::RasterFactory::create(te::rst::Grid* g, const std::vector<te::rst::BandProperty*> bands, const std::map<std::string, std::string>& /*rinfo*/, void* h, void (*deleter)(void*))
{
  throw Exception(TR_TERRALIB4("This driver doesn't support raster creation!"));
}

te::rst::Raster* terralib4::RasterFactory::build()
{
  return new Raster;
}

std::map<std::string, std::string> terralib4::RasterFactory::getCapabilities() const
{
  std::map<std::string, std::string> capabilities;

  capabilities["supported_formats"] = "tiff,jpeg";

  return capabilities;
}
