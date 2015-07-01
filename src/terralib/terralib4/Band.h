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
  \file terralib/terralib4/Band.h

  \brief Band implementation for TerraLib 4.x.
*/

#ifndef __TERRALIB_TERRALIB4_INTERNAL_BAND_H
#define __TERRALIB_TERRALIB4_INTERNAL_BAND_H

// TerraLib
#include "../raster/Band.h"

class TeRaster;

namespace terralib4
{
  class Raster;

  class Band : public te::rst::Band
  {
    public:

      Band(Raster* parent, TeRaster* iraster, te::rst::BandProperty* bp, std::size_t idx);

      ~Band();

      te::rst::Raster* getRaster() const;

      void getValue(unsigned int c, unsigned int r, double& value) const;

      void setValue(unsigned int c, unsigned int r, const double value);

      void getIValue(unsigned int c, unsigned int r, double& value) const;

      void setIValue(unsigned int c, unsigned int r, const double value);

      void read(int x, int y, void* buffer) const;

      void* read(int x, int y);

      void write(int x, int y, void* buffer);

    private:

      class Impl;

      Impl* m_pImpl;
  };

}   // end namespace terralib4

#endif  // __TERRALIB_TERRALIB4_INTERNAL_BAND_H
