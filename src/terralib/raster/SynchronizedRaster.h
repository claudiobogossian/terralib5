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
  \file terralib/raster/SynchronizedRaster.h

  \brief An adapter class to allow concurrent access to raster data by multiple threads.
*/

#ifndef __TERRALIB_RASTER_INTERNAL_SYNCHRONIZEDRASTER_H
#define __TERRALIB_RASTER_INTERNAL_SYNCHRONIZEDRASTER_H

// TerraLib
#include "Raster.h"
#include "RasterSynchronizer.h"
#include "SynchronizedBand.h"
#include "SynchronizedBandBlocksManager.h"
#include "Config.h"

namespace te
{
  namespace rst
  {
    /*!
      \class SynchronizedRaster

      \brief An adapter class to allow concurrent access to raster data by multiple threads.
      
      \note One unique RasterSynchronizer must be instantiated on the main process. That RasterSynchronizer is used by each thread to instantiate multiple SynchronizedRaster instances.
      
      \note More efficient access can be achieved by following the bands internal blocking scheme.
    */
    class TERASTEREXPORT SynchronizedRaster: public Raster
    {
      public:
        
        /*!
          \brief Constructor.

          \param sync The raster synchronizer instance.

          \param maxMemPercentUsed The maximum free memory percentual to use valid range: [1:100].
          
          \note For the case where using the write raster access policy: The use of multiple cached blocks can cause deadlocks if multiple threads are locking blocks needed by other threads, use it with caution!
        */
        SynchronizedRaster( RasterSynchronizer& sync, const unsigned char maxMemPercentUsed );

        /*!
          \brief Constructor.

          \param sync The raster synchronizer instance.

          \param maxNumberOfCacheBlocks The maximum number of cache blocks.
          
          \note For the case where using the write raster access policy: The use of multiple cached blocks can cause deadlocks if multiple threads are locking blocks needed by other threads, use it with caution!
        */
        SynchronizedRaster( const unsigned int maxNumberOfCacheBlocks, RasterSynchronizer& sync );

        ~SynchronizedRaster();        

        std::map<std::string, std::string> getInfo() const;

        inline std::size_t getNumberOfBands() const
        {
          return m_bands.size();
        };

        int getBandDataType(std::size_t i) const;        

        inline const te::rst::Band* getBand(std::size_t i) const
        {
          assert( i < m_bands.size() );
          return m_bands[ i ];
        };

        inline te::rst::Band* getBand(std::size_t i)
        {
          assert( i < m_bands.size() );
          return m_bands[ i ];
        };

        inline const te::rst::Band& operator[](std::size_t i) const
        {
          assert( i < m_bands.size() );
          return *(m_bands[ i ]);
        };        

        inline te::rst::Band& operator[](std::size_t i)
        {
          assert( i < m_bands.size() );
          return *(m_bands[ i ]);
        };

        te::dt::AbstractData* clone() const;

      protected:

        /*! \brief Free all allocated internal resources and go back to the initial state. */
        void free();

      private :

        SynchronizedRaster();

        SynchronizedRaster(te::rst::Grid* grid, te::common::AccessPolicy p = te::common::RAccess);
        
        void open(const std::map<std::string, std::string>& rinfo, te::common::AccessPolicy p = te::common::RAccess);

      protected :
        
        SynchronizedBandBlocksManager m_blocksManager; //!< Internal blocks manager.

        std::vector< SynchronizedBand* > m_bands; //!< Internal raster bands.
    };

  } // end namespace rst
}   // end namespace te

#endif //__TERRALIB_RASTER_INTERNAL_SYNCHRONIZEDRASTER_H
