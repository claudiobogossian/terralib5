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
  \file terralib/memory/CachedBand.h
 
  \brief RAM cached and tiled raster band.
*/

#ifndef __TERRALIB_MEMORY_INTERNAL_CACHEDBAND_H
#define __TERRALIB_MEMORY_INTERNAL_CACHEDBAND_H

// TerraLib
#include "Config.h"
#include "CachedBandBlocksManager.h"
#include "../raster/Band.h"

#include <cstddef>
#include <cassert>

namespace te
{  
  namespace mem
  {
// Forward declaration
    class Raster;

    /*!
      \class CachedBand

      \brief RAM cached and tiled raster band.
      
      \details A cache adaptor to an external existent raster band that must always be avaliable.
    */
    class TEMEMORYEXPORT CachedBand : public te::rst::Band
    {
      public:

        /*!
          \brief Constructor.

          \param blocksManager The blocks manager where to read/write data.
          \param idx The band index.
        */        
        CachedBand( CachedBandBlocksManager& blocksManager, std::size_t idx );

        ~CachedBand();

        inline te::rst::Raster* getRaster() const
        {
          return m_blocksManager.getRaster();
        };

        void getValue(unsigned int c, unsigned int r, double& value) const;

        void setValue(unsigned int c, unsigned int r, const double value);

        void getIValue(unsigned int c, unsigned int r, double& value) const;

        void setIValue(unsigned int c, unsigned int r, const double value);

        inline void read(int x, int y, void* buffer) const
        {
          assert( m_blocksManager.isInitialized() );
          m_blocksManager.read( m_idx, x, y, buffer );
        };

        inline void* read(int x, int y)
        {
          assert( m_blocksManager.isInitialized() );
          return m_blocksManager.read( m_idx, x, y );
        };

        void write(int x, int y, void* buffer)
        {
          assert( m_blocksManager.isInitialized() );
          m_blocksManager.write( m_idx, x, y, buffer );
        };

      protected :
        
        std::size_t m_idx;
        
        CachedBandBlocksManager& m_blocksManager;
        
        static CachedBandBlocksManager m_dummyBlocksManager;
      
      private :

        CachedBand();
        
        CachedBand(const CachedBand& );
    };

  } // end namespace mem
}   // end namespace te

#endif  // __TERRALIB_MEMORY_INTERNAL_CACHEDBAND_H

