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
  \file terralib/terralib4/Raster.h

  \brief Raster implementaton for TerraLib 4.x.
*/

#ifndef __TERRALIB_TERRALIB4_INTERNAL_RASTER_H
#define __TERRALIB_TERRALIB4_INTERNAL_RASTER_H

// TerraLib
#include "../raster/Raster.h"

class TeRaster;

namespace terralib4
{
  class Raster: public te::rst::Raster
  {
    public:

      Raster(TeRaster* iraster);

      Raster(te::rst::Grid* grid, te::common::AccessPolicy p = te::common::RAccess);

      ~Raster();

      void open(const std::map<std::string, std::string>& rinfo,
                te::common::AccessPolicy p = te::common::RAccess);

      std::map<std::string, std::string> getInfo() const;

      std::size_t getNumberOfBands() const;

      int getBandDataType(std::size_t i) const;

      const te::rst::Band* getBand(std::size_t i) const;

      te::rst::Band* getBand(std::size_t i);

      const te::rst::Band& operator[](std::size_t i) const;

      te::rst::Band& operator[](std::size_t i);

      te::dt::AbstractData* clone() const;

      bool createMultiResolution( const unsigned int levels, const te::rst::InterpolationMethod interpMethod )
      {
        return false;
      };
      
      bool removeMultiResolution() { return false; };
      
      unsigned int getMultiResLevelsCount() const
      {
        return 0;        
      }
      
      te::rst::Raster* getMultiResLevel( const unsigned int level ) const
      {
        return 0;         
      }

    private:

      class Impl;

      Impl* m_pImpl;
  };

}   // end namespace terralib4

#endif //__TERRALIB_TERRALIB4_INTERNAL_RASTER_H
