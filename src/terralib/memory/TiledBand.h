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
  \file terralib/memory/TiledBand.h
 
  \brief A tiled band implementation for the In-Memory Raster.
*/

#ifndef __TERRALIB_MEMORY_INTERNAL_TILEDBAND_H
#define __TERRALIB_MEMORY_INTERNAL_TILEDBAND_H

// TerraLib
#include "../raster/Band.h"
#include "../raster/BlockUtils.h"
#include "Config.h"

namespace te
{
  namespace mem
  {
// Forward declaration
    class Raster;

    /*!
      \class Band

      \brief A tiled band implementation for the In-Memory Raster.
    */
    class TEMEMORYEXPORT TiledBand : public te::rst::Band
    {
      public:

        TiledBand(Raster* r, te::rst::BandProperty* p, std::size_t idx);

        TiledBand(const TiledBand& rhs);

        ~TiledBand();

        te::rst::Raster* getRaster() const;

        TiledBand& operator=(const TiledBand& rhs);

        void getValue(unsigned int c, unsigned int r, double& value) const;

        void setValue(unsigned int c, unsigned int r, const double value);

        void getIValue(unsigned int c, unsigned int r, double& value) const;

        void setIValue(unsigned int c, unsigned int r, const double value);

        void read(int x, int y, void* buffer) const;

        void* read(int x, int y);

        void write(int x, int y, void* buffer);

        /*!
          \note In-Memory driver extended method.
        */
        void setRaster(Raster* r);

      private:

        Raster* m_raster;
        unsigned char*** m_buff;                 //!< A internal block buffer.
        te::rst::GetBufferValueFPtr m_getBuff;   //!< A pointer to a function that helps to extract a double or complex value from a specific buffer data type (char, int16, int32, float, ...).
        te::rst::GetBufferValueFPtr m_getBuffI;  //!< A pointer to a function that helps to extract the imaginary part value from a specific buffer data type (cint16, cint32, cfloat, cdouble).
        te::rst::SetBufferValueFPtr m_setBuff;   //!< A pointer to a function that helps to insert a double or complex value into a specific buffer data type (char, int16, int32, float, ...).
        te::rst::SetBufferValueFPtr m_setBuffI;  //!< A pointer to a function that helps to insert the imaginary part value into a specific buffer data type (cint16, cint32, cfloat, cdouble).
        int m_ncols;                             //!< Buffered number of cols.
        int m_nrows;                             //!< Buffered number of rows.
        int m_blksize;                           //!< The data block size.
        int m_nblksx;
        int m_nblksy;
        int m_blkw;
        int m_blkh;
    };

  }  // end namespace mem
}    // end namespace te

#endif  // __TERRALIB_MEMORY_INTERNAL_TILEDBAND_H
