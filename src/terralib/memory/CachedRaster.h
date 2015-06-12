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
  \file terralib/memory/CachedRaster.h

  \brief A RAM cache adaptor to an external existent raster that must always be avaliable.
*/

#ifndef __TERRALIB_MEMORY_INTERNAL_CACHEDRASTER_H
#define __TERRALIB_MEMORY_INTERNAL_CACHEDRASTER_H

// TerraLib
#include "../raster/Raster.h"
#include "CachedBand.h"
#include "CachedBandBlocksManager.h"
#include "Config.h"

// STL
#include <cassert>
#include <vector>

namespace te
{
  namespace mem
  {
    /*!
      \class CachedRaster

      \brief A RAM cache adaptor to an external existent raster that must always be avaliable.
      
      \ingroup mem

      \details A RAM cache adaptor to an external existent raster that must always be avaliable.
    */
    class TEMEMORYEXPORT CachedRaster: public te::rst::Raster
    {
      public:

        /*!
          \brief Constructor.

          \param rhs The external raster where the data will be read/written.

          \param maxMemPercentUsed The maximum free memory percentual to use valid range: [1:100].

          \param dataPrefetchThreshold The read-ahead data prefetch threshold (0-will disable prefetch, 1-data always prefetched, higher values will do prefetch when necessary).
        */
        CachedRaster( const te::rst::Raster& rhs, const unsigned char maxMemPercentUsed, 
                      const unsigned int dataPrefetchThreshold );

        /*!
          \brief Constructor.

          \param rhs The external raster where the data will be read/written.

          \param maxNumberOfCacheBlocks The maximum number of cache blocks.

          \param dataPrefetchThreshold The read-ahead data prefetch threshold (0-will disable prefetch, 1-data always prefetched, higher values will do prefetch when necessary).
        */
        CachedRaster( const unsigned int maxNumberOfCacheBlocks, const te::rst::Raster& rhs, 
                      const unsigned int dataPrefetchThreshold );

        ~CachedRaster();

        void open(const std::map<std::string, std::string>& rinfo, te::common::AccessPolicy p = te::common::RAccess);

        std::map<std::string, std::string> getInfo() const
        {
          assert( m_blocksManager.getRaster() );
          return m_blocksManager.getRaster()->getInfo();
        };

        inline std::size_t getNumberOfBands() const
        {
          return m_bands.size();
        };

        inline int getBandDataType(std::size_t i) const
        {
          assert( m_blocksManager.getRaster() );
          return m_blocksManager.getRaster()->getBandDataType( i );
        };        

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

      protected:

        /*! \brief Free all allocated internal resources and go back to the initial state. */
        void free();

      private :

        CachedRaster();

        CachedRaster(te::rst::Grid* grid, te::common::AccessPolicy p = te::common::RAccess);

      protected :

        std::vector< CachedBand* > m_bands; //!< Internal raster bands.

        CachedBandBlocksManager m_blocksManager; //!< Internal blocks manager.
    };

  } // end namespace mem
}   // end namespace te

#endif //__TERRALIB_MEMORY_INTERNAL_CACHEDRASTER_H
