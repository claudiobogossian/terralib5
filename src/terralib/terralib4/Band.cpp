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
  \file terralib/terralib4/Band.cpp

  \brief Band implementation for TerraLib 4.x.
*/

// TerraLib
#include "../common/Translator.h"
#include "Band.h"
#include "Exception.h"
#include "Raster.h"

// TerraLib 4.x
#include <terralib4/kernel/TeDecoderDatabase.h>
#include <terralib4/kernel/TeRaster.h>

class terralib4::Band::Impl
{
  public:

    Impl(Raster* parent, TeRaster* iraster);

    Raster* m_parent;
    TeRaster* m_raster;
};

terralib4::Band::Impl::Impl(Raster* parent, TeRaster* iraster)
  : m_parent(parent), m_raster(iraster)
{
}

terralib4::Band::Band(Raster* parent, TeRaster* iraster, te::rst::BandProperty* bp, std::size_t idx)
  : te::rst::Band(bp, idx), m_pImpl(0)
{
  m_pImpl = new Impl(parent, iraster);
}

terralib4::Band::~Band()
{
  delete m_pImpl;
}

te::rst::Raster* terralib4::Band::getRaster() const
{
  return m_pImpl->m_parent;
}

void terralib4::Band::getValue(unsigned int c, unsigned int r, double& value) const
{
  m_pImpl->m_raster->getElement(c, r, value, (int)m_idx);
}

void terralib4::Band::setValue(unsigned int /*c*/, unsigned int /*r*/, const double /*value*/)
{
  throw Exception(TE_TR("This method is not supported by TerraLib 4.x driver!"));
}

void terralib4::Band::getIValue(unsigned int /*c*/, unsigned int /*r*/, double& value) const
{
  value = 0.0;
}

void terralib4::Band::setIValue(unsigned int /*c*/, unsigned int /*r*/, const double /*value*/)
{
  throw Exception(TE_TR("This method is not supported by TerraLib 4.x driver!"));
}

void terralib4::Band::read(int x, int y, void* buffer) const
{
  TeDecoderDatabase* decDb = dynamic_cast<TeDecoderDatabase*>(m_pImpl->m_raster->decoder());

  std::string bdIdx = decDb->codifyId(x, y, (int)m_idx, 1, 0);

  decDb->getRasterBlock(bdIdx, buffer);
}

void* terralib4::Band::read(int, int)
{
  throw;
}

void terralib4::Band::write(int /*x*/, int /*y*/, void* /*buffer*/)
{
  throw Exception(TE_TR("This method is not supported by TerraLib 4.x driver!"));
}

