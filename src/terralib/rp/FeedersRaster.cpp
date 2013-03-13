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
  \file terralib/rp/FeedersRaster.cpp
  \brief Raster objects feeders.
*/

#include "FeedersRaster.h"
#include "Macros.h"
#include "../raster/RasterFactory.h"

namespace te
{
  namespace rp
  {
    FeederConstRasterVector::FeederConstRasterVector(
      const std::vector< const te::rst::Raster* > rasters )
    : m_currentOffset( 0 ), m_rasters( rasters )
    {
    };
    
    FeederConstRasterVector::FeederConstRasterVector()
    : m_currentOffset( 0 )
    {
    };    
    
    FeederConstRasterVector::~FeederConstRasterVector()
    {
    };
    
    te::rst::Raster const* FeederConstRasterVector::getCurrentObj() const
    {
      return ( m_currentOffset < m_rasters.size() ) ?
        m_rasters[ m_currentOffset ] : 0;            
    };    
    
    bool FeederConstRasterVector::moveNext()
    {
      if( m_currentOffset == m_rasters.size() )
      {
        return false;
      }
      else
      {
        ++m_currentOffset;
        return ( m_currentOffset != m_rasters.size() );
      }
    }
    
    void FeederConstRasterVector::reset()
    {
      m_currentOffset = 0;
    }
    
    unsigned int FeederConstRasterVector::getObjsCount() const
    {
      return (unsigned int) m_rasters.size();
    }
    
    unsigned int FeederConstRasterVector::getCurrentOffset() const
    {
      return m_currentOffset;
    }

    FeederConstRasterInfo::FeederConstRasterInfo(
      const std::vector< std::string >& rTypes,
      const std::vector< std::map< std::string, std::string > >& rInfos )
    : m_currentOffset( 0 ), m_rTypes( rTypes ), m_rInfos( rInfos )
    {
      TERP_TRUE_OR_THROW( rTypes.size() == rInfos.size(),
        "Invalid rasters info" )
      reset();
    };
    
    FeederConstRasterInfo::FeederConstRasterInfo()
    : m_currentOffset( 0 )
    {
    };    
    
    FeederConstRasterInfo::~FeederConstRasterInfo()
    {
    };
    
    te::rst::Raster const* FeederConstRasterInfo::getCurrentObj() const
    {
      return m_currentRasterPtr.get();
    };    
    
    bool FeederConstRasterInfo::moveNext()
    {
      if( m_currentOffset == m_rTypes.size() )
      {
        return false;
      }
      else
      {
        ++m_currentOffset;
        
        if( m_currentOffset == m_rTypes.size() )
        {
          m_currentRasterPtr.reset();
          return false;
        }
        else
        {
          m_currentRasterPtr.reset( te::rst::RasterFactory::open( 
            m_rTypes[ m_currentOffset ], m_rInfos[ m_currentOffset ], 
            te::common::RAccess ) );          
            
          if( m_currentRasterPtr.get() )
          {
            return true;
          }
          else
          {
            return false;
          }
        }
      }
    }
    
    void FeederConstRasterInfo::reset()
    {
      m_currentRasterPtr.reset();
      
      if( ! m_rTypes.empty() )
      {
        m_currentRasterPtr.reset( te::rst::RasterFactory::open( m_rTypes[ 0 ],
          m_rInfos[ 0 ], te::common::RAccess ) );          
          
        if( m_currentRasterPtr.get() )
        {
          m_currentOffset = 0;
        }
        else
        {
          m_currentOffset = m_rTypes.size();
        }
      }
    }
    
    unsigned int FeederConstRasterInfo::getObjsCount() const
    {
      return (unsigned int) m_rTypes.size();
    }
    
    unsigned int FeederConstRasterInfo::getCurrentOffset() const
    {
      return m_currentOffset;
    }

  } // end namespace rp
}   // end namespace te    

