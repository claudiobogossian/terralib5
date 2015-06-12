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
  \file terralib/raster/SynchronizedBand.h
 
  \brief Syncrhonized raster band.
*/

#ifndef __TERRALIB_RASTER_INTERNAL_SYNCHRONIZEDBAND_H
#define __TERRALIB_RASTER_INTERNAL_SYNCHRONIZEDBAND_H

// TerraLib
#include "Band.h"
#include "SynchronizedBandBlocksManager.h"
#include "BlockUtils.h"
#include "Config.h"

// Boost
#include <boost/noncopyable.hpp>

namespace te
{
  namespace rst
  {
    class SynchronizedRaster;
    
    /*!
      \class SynchronizedBand

      \brief Syncrhonized raster band.
      
      \note More efficient access can be achieved by following the bands internal blocking scheme.
    */
    class TERASTEREXPORT SynchronizedBand : public te::rst::Band, public boost::noncopyable
    {
      public:

        /*!
          \brief Constructor.

          \param blocksManager The blocks manager where to read/write data.
          \param raster The synchonized raster instance (parent raster).
          \param idx The band index.
        */
        SynchronizedBand( SynchronizedBandBlocksManager& blocksManager, SynchronizedRaster& raster, std::size_t idx );

        ~SynchronizedBand();

        inline te::rst::Raster* getRaster() const
        {
          return (te::rst::Raster*)(&m_syncRasterPtr);
        };

        void getValue(unsigned int c, unsigned int r, double& value) const;

        void setValue(unsigned int c, unsigned int r, const double value);

        void getIValue(unsigned int c, unsigned int r, double& value) const;

        void setIValue(unsigned int c, unsigned int r, const double value);

        void read(int x, int y, void* buffer) const;

        void* read(int x, int y);

        void write(int x, int y, void* buffer);

      private:

        SynchronizedBand();

      protected :
        
        static SynchronizedBandBlocksManager dummyBlocksManager;  //!< A global static dummy blocks manager.
        
        SynchronizedBandBlocksManager& m_blocksManager; //!< The external blocks manager.        

        SynchronizedRaster* m_syncRasterPtr; //!< The synchronized raster instance (parent raster).

        unsigned int m_blkWidth;                 //!< The current band blocks width
        unsigned int m_blkHeight;                //!< The current band blocks height
        unsigned int m_blkSizeBytes;             //!< The blocks size (bytes);

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

  } // end namespace rst
}     // end namespace te

#endif  // __TERRALIB_RASTER_INTERNAL_SYNCHRONIZEDBAND_H
