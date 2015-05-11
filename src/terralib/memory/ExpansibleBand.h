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
  \file terralib/memory/ExpansibleBand.h
 
  \brief Expansible raster band.
*/

#ifndef __TERRALIB_MEMORY_INTERNAL_EXPANSIBLEBAND_H
#define __TERRALIB_MEMORY_INTERNAL_EXPANSIBLEBAND_H

// TerraLib
#include "../raster/Band.h"
#include "../raster/BandProperty.h"
#include "../raster/BlockUtils.h"
#include "Config.h"
#include "ExpansibleBandBlocksManager.h"

// STL
#include <cassert>
#include <cstddef>

// Boost
#include <boost/noncopyable.hpp>

namespace te
{  
  namespace mem
  {
    /*!
      \class ExpansibleBand

      \brief Expansible raster band.

      \details A cache adaptor to an external existent raster band that must always be avaliable.
    */
    class TEMEMORYEXPORT ExpansibleBand : public te::rst::Band, public boost::noncopyable
    {
      public:

        /*!
          \brief Constructor.

          \param blocksManager The blocks manager where to read/write data.
          \param parentRaster  The parent raster pointer.
          \param idx           The band index.
        */
        ExpansibleBand( ExpansibleBandBlocksManager& blocksManager,
                        te::rst::Raster& parentRaster, 
                        const te::rst::BandProperty& bandProperty, std::size_t idx );

        ~ExpansibleBand();

        te::rst::Raster* getRaster() const
        {
          return m_parentRasterPtr;
        };

        void getValue(unsigned int c, unsigned int r, double& value) const;

        void setValue(unsigned int c, unsigned int r, const double value);

        void getIValue(unsigned int c, unsigned int r, double& value) const;

        void setIValue(unsigned int c, unsigned int r, const double value);

        void read(int x, int y, void* buffer) const;

        void* read(int x, int y)
        {
          return m_blocksManager.getBlockPointer( m_idx, x, y );
        };

        void write(int x, int y, void* buffer);

      private :

        ExpansibleBand();

      protected :

        static ExpansibleBandBlocksManager dummyBlocksManager; //!< A global static dummy blocks manager.

        ExpansibleBandBlocksManager& m_blocksManager; //!< The external blocks manager reference.

        te::rst::Raster* m_parentRasterPtr; //!< The parent raster ponter.

        unsigned int m_blkWidth; //!< Block width in pixels(for pixel access optimization purposes).

        unsigned int m_blkHeight; //!< Block width in pixels (for pixel access optimization purposes).

        unsigned int m_blkSizeBytes; //!< Blosk size bytes (for pixel access optimization purposes).

        te::rst::GetBufferValueFPtr m_getBuff;   //!< A pointer to a function that helps to extract a double or complex value from a specific buffer data type (char, int16, int32, float, ...).
        te::rst::GetBufferValueFPtr m_getBuffI;  //!< A pointer to a function that helps to extract the imaginary part value from a specific buffer data type (cint16, cint32, cfloat, cdouble).
        te::rst::SetBufferValueFPtr m_setBuff;   //!< A pointer to a function that helps to insert a double or complex value into a specific buffer data type (char, int16, int32, float, ...).
        te::rst::SetBufferValueFPtr m_setBuffI;  //!< A pointer to a function that helps to insert the imaginary part value into a specific buffer data type (cint16, cint32, cfloat, cdouble).

// Variable used by setValue/getValue methods
        mutable unsigned int m_setGetBlkX;
        mutable unsigned int m_setGetBlkY;
        mutable unsigned int m_setGetPos;
        mutable void* m_setGetBufPtr;
    };

  } // end namespace mem
}   // end namespace te

#endif  // __TERRALIB_MEMORY_INTERNAL_EXPANSIBLEBAND_H
