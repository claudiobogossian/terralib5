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
  \file terralib/raster/RasterSynchronizer.h

  \brief An access synchronizer to be used in SynchronizedRaster raster instances.
*/

#ifndef __TERRALIB_RASTER_INTERNAL_RASTEERSYNCHRONIZER_H
#define __TERRALIB_RASTER_INTERNAL_RASTEERSYNCHRONIZER_H

// TerraLib
#include "Raster.h"
#include "Config.h"
#include "../common/Enums.h"

// Boost
#include <boost/noncopyable.hpp>
#include <boost/thread/mutex.hpp>

// STL
#include <vector>

namespace te
{
  namespace rst
  {
    /*!
      \class RasterSynchronizer

      \brief An access synchronizer to be used in SynchronizedRaster raster instances.
    */
    class TERASTEREXPORT RasterSynchronizer: public boost::noncopyable
    {
      public:

        /*!
          \brief Constructor.

          \param raster The raster to synchronize.
        */
        RasterSynchronizer( Raster& raster, const te::common::AccessPolicy policy );
        
        ~RasterSynchronizer();
        
        /*!
          \brief Return the internal raster acces policy.
          \return Return the internal raster acces policy.
        */        
        inline te::common::AccessPolicy getPolicy() const
        {
          return m_policy; 
        }
        
        /*!
          \brief Return the internal raster reference.
          \return Return the internal raster reference.
        */        
        inline Raster& getRaster()
        {
          return m_raster; 
        }        

        /*!
          \brief Return the internal mutex reference.
          \return Return the internal mutex reference.
        */        
        inline boost::mutex& getMutex()
        {
          return m_mutex; 
        }        

        /*!
          \brief Increment a block use counter.
          \param bandIdx Block band index.
          \param blockXIndex Block X index.
          \param blockYIndex Block Y index.
          \return true if OK, false if the increment could not be done.
        */            
        bool incrementBlockUseCounter( const unsigned int bandIdx,
          const unsigned int blockXIndex, const unsigned int blockYIndex );

        /*!
          \brief Decrement a block use counter.
          \param bandIdx Block band index.
          \param blockXIndex Block X index.
          \param blockYIndex Block Y index.
        */            
        void decrementBlockUseCounter( const unsigned int bandIdx,
          const unsigned int blockXIndex, const unsigned int blockYIndex );
          
        /*!
          \brief Return the current block use counter.
          \param bandIdx Block band index.
          \param blockXIndex Block X index.
          \param blockYIndex Block Y index.
          \param useCounter The current block use counter.
        */            
        void getBlockUseCounter( const unsigned int bandIdx,
          const unsigned int blockXIndex, const unsigned int blockYIndex,
          unsigned int& useCounter );

      protected:
        
        /*! 
          \brief Blocks use counter type definition. 
          \note Indexed as [band][blockYIndex][blockXIndex]
        */
        typedef std::vector< std::vector< std::vector< unsigned int > > > BlocksUseCounterT;        
        
        te::common::AccessPolicy m_policy; //!< The access policy used on the given input raster.
        
        Raster& m_raster; //!< The input raster.
        
        boost::mutex m_mutex; //!< General sync mutex;
        
        BlocksUseCounterT m_blocksUseCounters; //!< blocks use counter.

    };

  } // end namespace rst
}   // end namespace te

#endif //__TERRALIB_RASTER_INTERNAL_RASTEERSYNCHRONIZER_H
