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
#include "Raster.h"

terralib4::Raster::Raster()
{
  throw;
}

terralib4::Raster::Raster(te::rst::Grid* grid, te::common::AccessPolicy p)
{
  throw;
}

terralib4::Raster::~Raster()
{
  throw;
}

void terralib4::Raster::open(const std::map<std::string, std::string>& rinfo,
                             te::common::AccessPolicy p)
{
  throw;
}

std::map<std::string, std::string> terralib4::Raster::getInfo() const
{
  throw;
}

std::size_t terralib4::Raster::getNumberOfBands() const
{
  throw;
}

int terralib4::Raster::getBandDataType(std::size_t i) const
{
  throw;
}

const te::rst::Band* terralib4::Raster::getBand(std::size_t i) const
{
  throw;
}

te::rst::Band* terralib4::Raster::getBand(std::size_t i)
{
  throw;
}

const te::rst::Band& terralib4::Raster::operator[](std::size_t i) const
{
  throw;
}

te::rst::Band& terralib4::Raster::operator[](std::size_t i)
{
  throw;
}

te::dt::AbstractData* terralib4::Raster::clone() const
{
  throw;
}
