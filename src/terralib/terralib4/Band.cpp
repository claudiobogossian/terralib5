/*  Copyright (C) 2011-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/terralib4/Band.cpp

  \brief Band implementation for TerraLib 4.x.
*/

// TerraLib
#include "Band.h"

terralib4::Band::Band(te::rst::BandProperty* p, std::size_t idx)
  : te::rst::Band(p, idx)
{
  throw;
}

terralib4::Band::~Band()
{
  throw;
}

te::rst::Raster* terralib4::Band::getRaster() const
{
  throw;
}

void terralib4::Band::getValue(unsigned int c, unsigned int r, double& value) const
{
  throw;
}

void terralib4::Band::setValue(unsigned int c, unsigned int r, const double value)
{
  throw;
}

void terralib4::Band::getIValue(unsigned int c, unsigned int r, double& value) const
{
  throw;
}

void terralib4::Band::setIValue(unsigned int c, unsigned int r, const double value)
{
  throw;
}

void terralib4::Band::read(int x, int y, void* buffer) const
{
  throw;
}

void* terralib4::Band::read(int x, int y)
{
  throw;
}

void terralib4::Band::write(int x, int y, void* buffer)
{
  throw;
}

