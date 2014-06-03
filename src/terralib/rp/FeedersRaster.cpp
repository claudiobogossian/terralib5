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
    : m_rType( rType )
    {
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
                    m_filesNames.push_back( dirIt->path().generic_string() );
                  }          
                }
              }
              else
              {
                m_filesNames.push_back( dirIt->path().generic_string() );
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
                    m_filesNames.push_back( dirIt->path().generic_string() );
                  }          
                }
              }
              else
              {
                m_filesNames.push_back( dirIt->path().generic_string() );
              }
            }
          
            ++dirIt;
          }
        }
      }
      
      if( sortFileNames ) std::sort( m_filesNames.begin(), m_filesNames.end() );
      
      m_currentOffset = m_filesNames.size();
      
      reset();
    }
    
    FeederConstRasterDirectory::FeederConstRasterDirectory()
    : m_currentOffset( 0 )
    {
    }
    
    FeederConstRasterDirectory::~FeederConstRasterDirectory()
    {
    }
    
    te::rst::Raster const* FeederConstRasterDirectory::getCurrentObj() const
    {
      return m_currentRasterPtr.get();
    }
    
    bool FeederConstRasterDirectory::moveNext()
    {
      if( m_currentOffset == m_filesNames.size() )
      {
        return false;
      }
      else
      {
        ++m_currentOffset;
        
        if( m_currentOffset == m_filesNames.size() )
        {
          m_currentRasterPtr.reset();
          return false;
        }
        else
        {
          std::map< std::string, std::string > mInfo;
          mInfo[ "URI" ] = m_filesNames[ m_currentOffset ];
          
          m_currentRasterPtr.reset( te::rst::RasterFactory::open( 
            m_rType, mInfo, 
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
    
    bool FeederConstRasterDirectory::moveTo( const unsigned int index )
    {
      if( index >= m_filesNames.size() )
      {
        return false;
      }
      else
      {
        m_currentOffset = index;
        
        std::map< std::string, std::string > mInfo;
        mInfo[ "URI" ] = m_filesNames[ m_currentOffset ];
        
        m_currentRasterPtr.reset( te::rst::RasterFactory::open( 
          m_rType, mInfo, 
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
    
    void FeederConstRasterDirectory::reset()
    {
      m_currentRasterPtr.reset();
      
      if( ! m_filesNames.empty() )
      {
        std::map< std::string, std::string > mInfo;
        mInfo[ "URI" ] = m_filesNames[ 0 ];        
        
        m_currentRasterPtr.reset( te::rst::RasterFactory::open( m_rType,
          mInfo, te::common::RAccess ) );          
          
        if( m_currentRasterPtr.get() )
        {
          m_currentOffset = 0;
        }
        else
        {
          m_currentOffset = m_filesNames.size();
        }
      }
    }
    
    unsigned int FeederConstRasterDirectory::getObjsCount() const
    {
      return (unsigned int) m_filesNames.size();
    }
    
    unsigned int FeederConstRasterDirectory::getCurrentOffset() const
    {
      return m_currentOffset;
    }    

  } // end namespace rp
}   // end namespace te    

