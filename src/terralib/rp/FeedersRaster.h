/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/rp/FeedersRaster.h
  \brief Raster objects feeders.
 */

#ifndef __TERRALIB_RP_INTERNAL_FEEDERSRASTER_H
#define __TERRALIB_RP_INTERNAL_FEEDERSRASTER_H

#include "Config.h"
#include "AbstractFeeder.h"
#include "../raster/Raster.h"

#include <vector>

namespace te
{
  namespace rp
  {
    /*!
      \class FeederConstRaster
      \brief Feeder from a input rasters.
      \ingroup RPModule
     */
    class TERPEXPORT FeederConstRaster : public AbstractFeeder< const te::rst::Raster >
    {
      public :
        
        virtual ~FeederConstRaster() {};
        
        //overloads
        virtual te::rst::Raster const* getCurrentObj() const = 0;
        virtual bool moveNext() = 0;
        virtual void reset() = 0;
        virtual unsigned int getObjsCount() const = 0;
        virtual unsigned int getCurrentOffset() const = 0;
        
      protected :
        
        FeederConstRaster() {};
    }; 
    
    /*!
      \class FeederConstRasterVector
      \brief A feeder from a input rasters vector;
      \ingroup RPModule
     */
    class TERPEXPORT FeederConstRasterVector : public FeederConstRaster
    {
      public :
        
        /*!
          \brief Constructor from a vector of input rasters pointers;
          \param rasters Input vector of rasters pointers.
          \note The given rasters must always be avaliable.
          \note The feeder WILL NOT ACQUIRE the pointers.
        */        
        FeederConstRasterVector( const std::vector< const te::rst::Raster* > rasters );
        
        ~FeederConstRasterVector();
        
        //overloads
        te::rst::Raster const* getCurrentObj() const;
        bool moveNext();
        void reset();
        unsigned int getObjsCount() const;
        unsigned int getCurrentOffset() const;
        
      protected :
        
        std::vector< const te::rst::Raster* >::size_type m_currentOffset;
        std::vector< const te::rst::Raster* > m_rasters;
        
        FeederConstRasterVector();
    };    
  } // end namespace rp
}   // end namespace te

#endif  // __TERRALIB_RP_INTERNAL_FEEDERSRASTER_H

