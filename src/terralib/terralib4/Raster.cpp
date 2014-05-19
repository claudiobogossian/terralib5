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
  \file terralib/terralib4/Raster.cpp

  \brief Raster implementaton for TerraLib 4.x.
*/

// TerraLib
#include "../common/Translator.h"
#include "../raster/BandProperty.h"
#include "../raster/Grid.h"
#include "../raster/RasterProperty.h"
#include "Band.h"
#include "Exception.h"
#include "Raster.h"
#include "Utils.h"

// Boost
#include <boost/ptr_container/ptr_vector.hpp>

// TerraLib 4.x
#include <kernel/TeRaster.h>

class terralib4::Raster::Impl
{
  public:

    Impl(TeRaster* iraster);

    TeRaster* m_raster;

    boost::ptr_vector<Band> m_bands;
};

terralib4::Raster::Impl::Impl(TeRaster* iraster)
  : m_raster(iraster)
{
}

terralib4::Raster::Raster(TeRaster* iraster)
  : m_pImpl(0)
{
  m_pImpl = new Impl(iraster);

  te::rst::RasterProperty* prop = Convert2T5(iraster->params());

  m_grid = new te::rst::Grid(*prop->getGrid());

  for(std::size_t i = 0; i != getNumberOfBands(); ++i)
  {
    m_pImpl->m_bands.push_back(new Band(this, iraster, prop->getBandProperties()[i], i));

    prop->getBandProperties()[i] = 0;
  }

  delete prop;
}

terralib4::Raster::Raster(te::rst::Grid* /*grid*/, te::common::AccessPolicy /*p*/)
{
  throw Exception(TE_TR("This method is not supported by TerraLib 4.x driver!"));
}

terralib4::Raster::~Raster()
{
  delete m_pImpl;
}

void terralib4::Raster::open(const std::map<std::string, std::string>& /*rinfo*/,
                             te::common::AccessPolicy /*p*/)
{
  throw Exception(TE_TR("This method is not supported by TerraLib 4.x driver!"));
}

std::map<std::string, std::string> terralib4::Raster::getInfo() const
{
  throw;
}

std::size_t terralib4::Raster::getNumberOfBands() const
{
  return m_pImpl->m_raster->nBands();
}

int terralib4::Raster::getBandDataType(std::size_t i) const
{
  TeDataType dt = m_pImpl->m_raster->params().dataType_[i];

  return Convert2T5(dt);
}

const te::rst::Band* terralib4::Raster::getBand(std::size_t i) const
{
  return &m_pImpl->m_bands[i];
}

te::rst::Band* terralib4::Raster::getBand(std::size_t i)
{
  return &m_pImpl->m_bands[i];
}

const te::rst::Band& terralib4::Raster::operator[](std::size_t i) const
{
  return m_pImpl->m_bands[i];
}

te::rst::Band& terralib4::Raster::operator[](std::size_t i)
{
  return m_pImpl->m_bands[i];
}

te::dt::AbstractData* terralib4::Raster::clone() const
{
  throw Exception(TE_TR("This method is not supported by TerraLib 4.x driver!"));
}
