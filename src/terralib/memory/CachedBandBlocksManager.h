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
  \file terralib/memory/CachedBandBlocksManager.h
 
  \brief RAM cached and tiled raster band blocks manager.
*/

#ifndef __TERRALIB_MEMORY_INTERNAL_CACHEDBANDBLOCKSMANAGER_H
#define __TERRALIB_MEMORY_INTERNAL_CACHEDBANDBLOCKSMANAGER_H

// TerraLib
#include "Config.h"
#include "../raster/Raster.h"

#include <boost/multi_array.hpp>

#include <vector>

namespace te
{  
  namespace mem
  {
    /*!
      \class CachedBandBlocksManager

      \brief RAM cached and tiled raster band blocks manager.
    */
    class TEMEMORYEXPORT CachedBandBlocksManager
    {
      public:

        CachedBandBlocksManager();

        ~CachedBandBlocksManager();
        
        /*!
          \brief Initialize this instance to an initial state.
          
          \param externalRaster The external raster where the data will be read/writed.
          
          \param maxMemPercentUsed The maximum free memory percentual to use valid range: [1:100].
          
          \param dataPrefetchThreshold The read-ahead data prefetch threshold (0-will disable prefetch, 1-data always prefetched, higher values will do prefetch when necessary).
          
          \return true if OK, false on errors.
        */        
        bool initialize( const te::rst::Raster& externalRaster, 
          const unsigned char maxMemPercentUsed, const unsigned int dataPrefetchThreshold );
        
        /*!
          \brief Returns true if this instance is initialized.
          
          \return true if this instance is initialized.
        */        
        inline bool isInitialized() const
        {
          return m_rasterPtr ? true : false;
        };

        /*!
          \note Free all allocated internal resources and go back to the initial state.
        */
        void free(); 
        
        /*!
          \brief Returns a pointer to the required data block.

          \param band The band index.
          
          \param x The block-id in x (or x-offset).
          
          \param y The block-id in y (or y-offset).

          \return a pointer to the required data block.
        */
        void* getBlockPointer(unsigned int band, unsigned int x, unsigned int y);
        
        
        /*! \brief Returns the associated raster. */
        te::rst::Raster* getRaster() const
        {
          return m_rasterPtr;
        };        
        
      protected :
        
        te::rst::Raster* m_rasterPtr;
        
        boost::multi_array<void*, 3> m_blocksPointers;
        
      private :
        
        CachedBandBlocksManager( const CachedBandBlocksManager& );
        
        const CachedBandBlocksManager& operator=( const CachedBandBlocksManager& );
    };

  } // end namespace mem
}   // end namespace te

#endif  // __TERRALIB_MEMORY_INTERNAL_CACHEDBANDBLOCKSMANAGER_H

