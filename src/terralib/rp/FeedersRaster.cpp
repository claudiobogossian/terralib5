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
  \file terralib/rp/FeedersRaster.cpp
  \brief Raster objects feeders.
*/

#include "FeedersRaster.h"
#include "Macros.h"
#include "../raster/RasterFactory.h"
#include "../raster/Grid.h"
#include "../geometry/LinearRing.h"
#include "../common/Exception.h"

#include <boost/filesystem.hpp>

namespace te
{
  namespace rp
  {
    // -----------------------------------------------------------------------
    
    FeederConstRasterVector::FeederConstRasterVector(
      const std::vector< const te::rst::Raster* > rasters )
    : m_currentOffset( 0 ), m_rasters( rasters )
    {
    }
    
    FeederConstRasterVector::FeederConstRasterVector()
    : m_currentOffset( 0 )
    {
    }
    
    FeederConstRasterVector::~FeederConstRasterVector()
    {
    }
    
    te::rst::Raster const* FeederConstRasterVector::getCurrentObj() const
    {
      return ( m_currentOffset < m_rasters.size() ) ?
        m_rasters[ m_currentOffset ] : 0;            
    }
    
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
    
    bool FeederConstRasterVector::moveTo( const unsigned int index )
    {
      if( index >= m_rasters.size() )
      {
        return false;
      }
      else
      {
        m_currentOffset = index;
        return true;
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
    
    // -----------------------------------------------------------------------

    FeederConstRasterInfo::FeederConstRasterInfo(
      const std::vector< std::string >& rTypes,
      const std::vector< std::map< std::string, std::string > >& rInfos )
    : m_currentOffset( 0 ), m_rTypes( rTypes ), m_rInfos( rInfos )
    {
      TERP_TRUE_OR_THROW( rTypes.size() == rInfos.size(),
        "Invalid rasters info" )
      reset();
    }
    
    FeederConstRasterInfo::FeederConstRasterInfo()
    : m_currentOffset( 0 )
    {
    }
    
    FeederConstRasterInfo::~FeederConstRasterInfo()
    {
    }
    
    te::rst::Raster const* FeederConstRasterInfo::getCurrentObj() const
    {
      return m_currentRasterPtr.get();
    }
    
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
    
    bool FeederConstRasterInfo::moveTo( const unsigned int index )
    {
      if( index >= m_rInfos.size() )
      {
        return false;
      }
      else if( index == m_currentOffset )
      {
        return true;
      }
      else
      {
        m_currentOffset = index;
        
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
    
    // -----------------------------------------------------------------------
    
    FeederConstRasterDirectory::FeederConstRasterDirectory(
      const std::string& directoryName,
      const bool recursive,
      const std::string& rType,
      const bool sortFileNames,
      const std::vector< std::string >& fileExtensions )
    {
      TERP_TRUE_OR_THROW( initialize( directoryName, recursive, rType,
        sortFileNames, fileExtensions, 0, false ), "Feeder initialization error" );
    }
    
    FeederConstRasterDirectory::FeederConstRasterDirectory(
      const std::string& directoryName,
      const bool recursive,
      const std::string& rType,
      const bool sortFileNames,
      const std::vector< std::string >& fileExtensions,
      te::gm::Geometry const * const restrictionGeomPtr,
      const bool ignoreInvalidRasterFiles )
    {
      TERP_TRUE_OR_THROW( initialize( directoryName, recursive, rType,
        sortFileNames, fileExtensions, restrictionGeomPtr, ignoreInvalidRasterFiles ), 
        "Feeder initialization error" );
    }    
    
    FeederConstRasterDirectory::FeederConstRasterDirectory()
    : m_selectedRasterIndexesOffset( 0 )
    {
    }
    
    FeederConstRasterDirectory::~FeederConstRasterDirectory()
    {
    }
    
    const std::string& FeederConstRasterDirectory::getCurrentRasterFileName()
    {
      if( m_selectedRasterIndexesOffset < m_selectedRastersIndexes.size() )
      {
        return m_allRasterFileNames[ m_selectedRastersIndexes[ m_selectedRasterIndexesOffset ] ];
      }
      else
      {
        static std::string emptyStr;
        return emptyStr;
      }
    }
    
    te::rst::Raster const* FeederConstRasterDirectory::getCurrentObj() const
    {
      return m_currentRasterPtr.get();
    }
    
    bool FeederConstRasterDirectory::moveNext()
    {
      m_currentRasterPtr.reset();
      
      std::map< std::string, std::string > mInfo;
      
      std::size_t nextSelectedRasterIndexesOffset = m_selectedRasterIndexesOffset + 1;
      
      while( nextSelectedRasterIndexesOffset < m_selectedRastersIndexes.size() )
      {
        mInfo[ "URI" ] = m_allRasterFileNames[ m_selectedRastersIndexes[ 
          nextSelectedRasterIndexesOffset ] ];
        
        try
        {
          m_currentRasterPtr.reset( te::rst::RasterFactory::open( 
            m_rType, mInfo, 
            te::common::RAccess ) );          
        }
        catch( const te::common::Exception& excep )
        {
          if( !m_ignoreInvalidRasterFiles )
          {
            m_selectedRasterIndexesOffset = m_selectedRastersIndexes.size();
            m_currentRasterPtr.reset();
            TERP_LOG_AND_RETURN_FALSE( "Error:" + std::string( excep.what() ) );
          }
        }
        
        if( m_currentRasterPtr.get() )
        {
          break;
        }
        
        ++nextSelectedRasterIndexesOffset;
      }
      
      m_selectedRasterIndexesOffset = nextSelectedRasterIndexesOffset;
      
      if( m_currentRasterPtr.get() )
      {
        return true;
      } 
      else
      {
        return false;
      }
    }
    
    bool FeederConstRasterDirectory::moveTo( const unsigned int index )
    {
      if( index >= m_selectedRastersIndexes.size() )
      {
        m_currentRasterPtr.reset();
        return false;
      }
      else
      {
        std::map< std::string, std::string > mInfo;
        mInfo[ "URI" ] = m_allRasterFileNames[ m_selectedRastersIndexes[
          index ] ];

        try
        {
          m_currentRasterPtr.reset( te::rst::RasterFactory::open( 
            m_rType, mInfo, 
            te::common::RAccess ) );  
        }
        catch( const te::common::Exception& excep )
        {
          TERP_LOGWARN( "Error:" + std::string( excep.what() ) );
          m_currentRasterPtr.reset();
        }          
        
          
        if( m_currentRasterPtr.get() )
        {
          m_selectedRasterIndexesOffset = index;
          return true;
        }
        else
        {
          return false;
        }
      }
    }     
    
    void FeederConstRasterDirectory::reset()
    {
      m_currentRasterPtr.reset();
      
      std::map< std::string, std::string > mInfo;
      std::size_t nextSelectedRasterIndexesOffset = 0;
      
      while( nextSelectedRasterIndexesOffset < m_selectedRastersIndexes.size() )
      {
        mInfo[ "URI" ] = m_allRasterFileNames[ m_selectedRastersIndexes[ 
          nextSelectedRasterIndexesOffset ] ];        
        
        try
        {
          m_currentRasterPtr.reset( te::rst::RasterFactory::open( m_rType,
            mInfo, te::common::RAccess ) );          
        }
        catch( const te::common::Exception& excep )
        {
          if( !m_ignoreInvalidRasterFiles )
          {
            TERP_LOGWARN( "Error:" + std::string( excep.what() ) );
            m_currentRasterPtr.reset();
            m_selectedRasterIndexesOffset = m_selectedRastersIndexes.size();
            return;
          }
        }        
          
        if( m_currentRasterPtr.get() )
        {
          break;
        }
        
        ++nextSelectedRasterIndexesOffset;
      }
      
      m_selectedRasterIndexesOffset = nextSelectedRasterIndexesOffset;
    }
    
    unsigned int FeederConstRasterDirectory::getObjsCount() const
    {
      return (unsigned int) m_selectedRastersIndexes.size();
    }
    
    unsigned int FeederConstRasterDirectory::getCurrentOffset() const
    {
      return m_selectedRasterIndexesOffset;
    }  
    
    void FeederConstRasterDirectory::applyGeometryRestriction( const te::gm::Geometry& restrictionGeom )
    {
       std::auto_ptr< te::dt::AbstractData > cloneResult( restrictionGeom.clone() );
       TERP_TRUE_OR_THROW( dynamic_cast< te::gm::Geometry* >( cloneResult.get() ), 
         "Invalid geometry" );
       
       m_restrictionGeomPtr.reset( (te::gm::Geometry*)cloneResult.release() );
       
       // reading all bounding boxes
       
       if( m_allRastersBoundingBoxes.size() != m_allRasterFileNames.size() )
       {
         m_allRastersBoundingBoxes.clear();
         
         te::gm::Polygon auxPolygon( 0, te::gm::PolygonType, 0 );
         te::gm::LinearRing* auxLinearRingPtr = 0;
         std::map< std::string, std::string > mInfo;
         std::auto_ptr< te::rst::Raster > rasterPtr;
         te::gm::Envelope const* rasterExtPtr = 0;
         
         for( unsigned int allRasterFileNamesIdx = 0 ; allRasterFileNamesIdx <
           m_allRasterFileNames.size() ; ++allRasterFileNamesIdx )
         {
            mInfo[ "URI" ] = m_allRasterFileNames[ allRasterFileNamesIdx ];        
            
            try
            {
              rasterPtr.reset( te::rst::RasterFactory::open( m_rType,
                mInfo, te::common::RAccess ) );          
              TERP_TRUE_OR_THROW( rasterPtr.get(), "Invalid raster" );
            }
            catch( const te::common::Exception& excep )
            {
              rasterPtr.reset();
              
              if( m_ignoreInvalidRasterFiles )
              {
                TERP_LOGWARN( "Error:" + std::string( excep.what() ) );
              }
              else
              {
                TERP_LOG_AND_THROW( "Error:" + std::string( excep.what() ) );
              }
            }
            
            if( rasterPtr.get() )
            {
              rasterExtPtr = rasterPtr->getGrid()->getExtent();
              
              auxLinearRingPtr = new te::gm::LinearRing(5, te::gm::LineStringType);
              auxLinearRingPtr->setPoint( 0, rasterExtPtr->m_llx, 
                rasterExtPtr->m_ury );
              auxLinearRingPtr->setPoint( 1, rasterExtPtr->m_urx, 
                rasterExtPtr->m_ury );
              auxLinearRingPtr->setPoint( 2, rasterExtPtr->m_urx, 
                rasterExtPtr->m_lly );
              auxLinearRingPtr->setPoint( 3, rasterExtPtr->m_llx, 
                rasterExtPtr->m_lly );
              auxLinearRingPtr->setPoint( 4, rasterExtPtr->m_llx, 
                rasterExtPtr->m_ury );
              auxLinearRingPtr->setSRID( rasterPtr->getSRID() );
              
              auxPolygon.clear();
              auxPolygon.push_back( auxLinearRingPtr );
              
              auxPolygon.setSRID( rasterPtr->getSRID() );
              
              m_allRastersBoundingBoxes.push_back( auxPolygon );
            }
         }
       }
       
       // Selecting the intersected bounding boxes
       
       m_selectedRastersIndexes.clear();
       
       for( unsigned int allRastersBoundingBoxesIdx = 0 ; allRastersBoundingBoxesIdx <
           m_allRastersBoundingBoxes.size() ; ++allRastersBoundingBoxesIdx )
       {
         if( m_restrictionGeomPtr->getSRID() != m_allRastersBoundingBoxes[
           allRastersBoundingBoxesIdx ].getSRID() )
         {
           m_restrictionGeomPtr->transform( m_allRastersBoundingBoxes[
             allRastersBoundingBoxesIdx ].getSRID() );
         }
         
         if( m_restrictionGeomPtr->intersects( &( m_allRastersBoundingBoxes[
           allRastersBoundingBoxesIdx ] ) ) )
         {
           m_selectedRastersIndexes.push_back( allRastersBoundingBoxesIdx );
         }
       }
       
       m_selectedRasterIndexesOffset = m_selectedRastersIndexes.size();
     
       reset();
    }
    
    bool FeederConstRasterDirectory::initialize(
      const std::string& directoryName,
      const bool recursive,
      const std::string& rType,
      const bool sortFileNames,
      const std::vector< std::string >& fileExtensions,
      te::gm::Geometry const * const restrictionGeomPtr,
      const bool ignoreInvalidRasterFiles )
    {
      m_ignoreInvalidRasterFiles = ignoreInvalidRasterFiles;
      
      // updating m_allRasterFileNames
      
      {
        m_allRasterFileNames.clear();
        
        m_rType = rType;
        const std::vector< std::string >::size_type fileExtensionsSize = 
          fileExtensions.size();
        std::vector< std::string >::size_type fileExtensionsIdx = 0;
        
        boost::filesystem::path directoryPath( directoryName );
        if( boost::filesystem::is_directory( directoryName ) )
        {
          if( recursive )
          {
            boost::filesystem::recursive_directory_iterator dirIt( directoryPath );
            const boost::filesystem::recursive_directory_iterator dirItE;
            std::string auxString;
          
            while( dirIt != dirItE )
            {
              if( boost::filesystem::is_regular_file( *dirIt ) )
              {
                if( fileExtensionsSize )
                {
                  for( fileExtensionsIdx = 0 ; fileExtensionsIdx < fileExtensionsSize ;
                    ++fileExtensionsIdx )
                  {
                    if( dirIt->path().extension().generic_string()
                        == fileExtensions[ fileExtensionsIdx ] )
                    {
                      m_allRasterFileNames.push_back( dirIt->path().generic_string() );
                    }          
                  }
                }
                else
                {
                  m_allRasterFileNames.push_back( dirIt->path().generic_string() );
                }
              }
            
              ++dirIt;
            }
          }
          else
          {
            boost::filesystem::directory_iterator dirIt( directoryPath );
            const boost::filesystem::directory_iterator dirItE;
          
            while( dirIt != dirItE )
            {
              if( boost::filesystem::is_regular_file( *dirIt ) )
              {
                if( fileExtensionsSize )
                {
                  for( fileExtensionsIdx = 0 ; fileExtensionsIdx < fileExtensionsSize ;
                    ++fileExtensionsIdx )
                  {
                    if( dirIt->path().extension().generic_string()
                        == fileExtensions[ fileExtensionsIdx ] )
                    {
                      m_allRasterFileNames.push_back( dirIt->path().generic_string() );
                    }          
                  }
                }
                else
                {
                  m_allRasterFileNames.push_back( dirIt->path().generic_string() );
                }
              }
            
              ++dirIt;
            }
          }
        }
        
        if( sortFileNames ) std::sort( m_allRasterFileNames.begin(), m_allRasterFileNames.end() );
      }
      
      // updating m_selectedRastersIndexes
      
      {
        m_selectedRastersIndexes.resize( m_allRasterFileNames.size() );
        
        for( std::vector< std::string >::size_type allRasterFileNamesIdx = 0 ;
          allRasterFileNamesIdx < m_allRasterFileNames.size() ;
          ++allRasterFileNamesIdx )
        {
          m_selectedRastersIndexes[ allRasterFileNamesIdx ] = allRasterFileNamesIdx;
        }
      }
      
      m_selectedRasterIndexesOffset = m_selectedRastersIndexes.size();
      
      // applying the restriction geometry
      
      if( restrictionGeomPtr )
      {
        applyGeometryRestriction( *restrictionGeomPtr );
      }
      else
      {
        reset();
      }
            
      return true;
    }    

  } // end namespace rp
}   // end namespace te    

