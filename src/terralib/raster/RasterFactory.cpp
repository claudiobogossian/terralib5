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
  \file terralib/raster/RasterFactory.cpp

  \brief This is the abstract factory for Rasters.
*/

// TerraLib
#include "../common/StringUtils.h"
#include "../common/Translator.h"
#include "Exception.h"
#include "RasterFactory.h"

// STL
#include <memory>

te::rst::Raster* te::rst::RasterFactory::make()
{
  return te::common::AbstractFactory<Raster, std::string>::make(TE_DEFAULT_RASTER_TYPE);
}

te::rst::Raster* te::rst::RasterFactory::make(const std::string& rType)
{
  std::string ucase = te::common::Convert2UCase(rType);

  return te::common::AbstractFactory<Raster, std::string>::make(ucase);
}

te::rst::Raster* te::rst::RasterFactory::make(Grid* g, const std::vector<BandProperty*> bands, const std::map<std::string, std::string>& rinfo, void* h, void (*deleter)(void*))
{
  std::map<std::string, std::string>::const_iterator it = rinfo.find("FORCE_MEM_DRIVER");

  if((it != rinfo.end()) &&
     (te::common::Convert2UCase(it->second) == "TRUE"))
    return make("MEM", g, bands, rinfo, h, deleter);

  return make(TE_DEFAULT_RASTER_TYPE, g, bands, rinfo, h, deleter);
}

te::rst::Raster* te::rst::RasterFactory::make(const std::string& rType, Grid* g, const std::vector<BandProperty*> bands, const std::map<std::string, std::string>& rinfo, void* h, void (*deleter)(void*))
{
  std::string ucase = te::common::Convert2UCase(rType);

  te::common::AbstractFactory<Raster, std::string>::dictionary_type& d = te::common::AbstractFactory<Raster, std::string>::getDictionary();

  RasterFactory* f = static_cast<RasterFactory*>(d.find(ucase));

  if(f == 0)
    throw Exception(TE_TR("Could not find concrete factory! Check if it was initialized!"));

  Raster* r = f->create(g, bands, rinfo, h, deleter);

  return r;
}

te::rst::Raster* te::rst::RasterFactory::make(const std::map<std::string, std::string>& rinfo, void* h, void (*deleter)(void*))
{
  return make(TE_DEFAULT_RASTER_TYPE, 0, std::vector<BandProperty*>(), rinfo, h, deleter);
}

te::rst::Raster* te::rst::RasterFactory::make(const std::string& rType, const std::map<std::string, std::string>& rinfo, void* h, void (*deleter)(void*))
{
  return make(rType, 0, std::vector<BandProperty*>(), rinfo, h, deleter);
}

te::rst::Raster* te::rst::RasterFactory::open(const std::map<std::string, std::string>& rinfo, te::common::AccessPolicy p)
{
  return open(TE_DEFAULT_RASTER_TYPE, rinfo, p);
}

te::rst::Raster* te::rst::RasterFactory::open(const std::string& rType, const std::map<std::string, std::string>& rinfo, te::common::AccessPolicy p)
{
  std::string ucase = te::common::Convert2UCase(rType);

  te::common::AbstractFactory<Raster, std::string>::dictionary_type& d = te::common::AbstractFactory<Raster, std::string>::getDictionary();

  RasterFactory* f = static_cast<RasterFactory*>(d.find(ucase));

  if(f == 0)
    throw Exception(TE_TR("Could not find concrete factory! Check if it was initialized!"));

  return f->iOpen(rinfo, p);
}

te::rst::Raster* te::rst::RasterFactory::open(const std::string& key, const std::string& value, te::common::AccessPolicy p)
{
  return open(TE_DEFAULT_RASTER_TYPE, key, value, p);
}

te::rst::Raster* te::rst::RasterFactory::open(const std::string& rType, const std::string& key, const std::string& value, te::common::AccessPolicy p)
{
  std::map<std::string, std::string> rinfo;

  rinfo[key] = value;

  return open(rType, rinfo, p);
}

te::rst::RasterFactory::RasterFactory(const std::string& factoryKey)
  : te::common::AbstractFactory<te::rst::Raster, std::string>(factoryKey)
{
}

te::rst::Raster* te::rst::RasterFactory::iOpen(const std::map<std::string, std::string>& rinfo, te::common::AccessPolicy p)
{
  std::auto_ptr<Raster> r(make(getKey()));

  r->open(rinfo, p);

  return r.release();
}

