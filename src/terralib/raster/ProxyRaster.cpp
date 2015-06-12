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
  \file terralib/raster/ProxyRaster.cpp

  \brief A proxy class for bands of several rasters over the same region.
*/

// TerraLib
#include "../common/Translator.h"
#include "Band.h"
#include "BandProperty.h"
#include "Exception.h"
#include "Grid.h"
#include "ProxyRaster.h"

// STL
#include <cassert>
#include <utility>

te::rst::ProxyRaster::ProxyRaster(const std::vector<RasterPtr>& rasters)
  : m_rasters(rasters)
{
// TODO: check if all rasters have the same grid!
  assert(rasters.empty() == false);

  const std::size_t nrasters = rasters.size();

  for(std::size_t i = 0; i != nrasters; ++i)
  {
    RasterPtr r(rasters[i]);

    const std::size_t nbands = r->getNumberOfBands();

    for(std::size_t b = 0; b != nbands; ++b)
    {
      m_bands.push_back(r->getBand(b));
    }
  }

  assert(m_grid == 0);

  m_grid = new Grid(*(rasters[0]->getGrid()));
}

te::rst::ProxyRaster::ProxyRaster(const ProxyRaster& /*rhs*/)
{
  throw Exception(TE_TR("Not implemented yet!"));
}

te::rst::ProxyRaster::~ProxyRaster()
{
}

void te::rst::ProxyRaster::swap(std::size_t first, std::size_t second)
{
  std::swap(m_bands[first], m_bands[second]);
  std::swap(m_rasters[first], m_rasters[second]);
}

void te::rst::ProxyRaster::open(const std::map<std::string, std::string>& /*rinfo*/, te::common::AccessPolicy /*p*/)
{
}

std::map<std::string, std::string> te::rst::ProxyRaster::getInfo() const
{
  return m_rinfo;
}

std::size_t te::rst::ProxyRaster::getNumberOfBands() const
{
  return m_bands.size();
}

int te::rst::ProxyRaster::getBandDataType(std::size_t i) const
{
  assert(i < m_bands.size());
  assert(m_bands[i]);
  assert(m_bands[i]->getProperty());

  return m_bands[i]->getProperty()->getType();
}

const te::rst::Band* te::rst::ProxyRaster::getBand(std::size_t i) const
{
  assert(i < m_bands.size());

  return m_bands[i];
}

te::rst::Band* te::rst::ProxyRaster::getBand(std::size_t i)
{
  assert(i < m_bands.size());

  return m_bands[i];
}

const te::rst::Band& te::rst::ProxyRaster::operator[](std::size_t i) const
{
  assert(i < m_bands.size());
  assert(m_bands[i]);

  return *(m_bands[i]);
}

te::rst::Band& te::rst::ProxyRaster::operator[](std::size_t i)
{
  assert(i < m_bands.size());
  assert(m_bands[i]);

  return *(m_bands[i]);
}

