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

#include <string>
#include <vector>
#include <map>
#include <memory>

namespace te
{
  namespace rp
  {
    /*!
      \class FeederConstRaster
      \brief Feeder from a input rasters.
     */
    class TERPEXPORT FeederConstRaster : public AbstractFeeder< const te::rst::Raster >
    {
      public :
        
        virtual ~FeederConstRaster() {};
        
        //overloads
        virtual te::rst::Raster const* getCurrentObj() const = 0;
        virtual bool moveNext() = 0;
        virtual bool moveTo( const unsigned int index ) = 0;
        virtual void reset() = 0;
        virtual unsigned int getObjsCount() const = 0;
        virtual unsigned int getCurrentOffset() const = 0;
        
      protected :
        
        FeederConstRaster() {};
    }; 
    
    /*!
      \class FeederConstRasterVector
      \brief A feeder from a input rasters vector;
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
        bool moveTo( const unsigned int index );
        void reset();
        unsigned int getObjsCount() const;
        unsigned int getCurrentOffset() const;
        
      protected :
        
        std::vector< const te::rst::Raster* >::size_type m_currentOffset;
        std::vector< const te::rst::Raster* > m_rasters;
        
        FeederConstRasterVector();
    };  
    
    /*!
      \class FeederConstRasterFileNames
      \brief A feeder from a vector of input rasters infos.
     */
    class TERPEXPORT FeederConstRasterInfo : public FeederConstRaster
    {
      public :
        
        /*!
          \brief Constructor from a vector of input rasters infos;
          \param rTypes The name of the specific drivers to instantiate each raster. 
          \param rInfos The necessary information to instantiate each raster. 
          \note The given rasters must always be avaliable.
        */        
        FeederConstRasterInfo( const std::vector< std::string >& rTypes,
          const std::vector< std::map< std::string, std::string > >& rInfos );
        
        ~FeederConstRasterInfo();
        
        //overloads
        te::rst::Raster const* getCurrentObj() const;
        bool moveNext();
        bool moveTo( const unsigned int index );
        void reset();
        unsigned int getObjsCount() const;
        unsigned int getCurrentOffset() const;
        
      protected :
        
        std::vector< std::string >::size_type m_currentOffset;
        std::vector< std::string > m_rTypes;
        std::vector< std::map< std::string, std::string > > m_rInfos;
        std::auto_ptr< te::rst::Raster > m_currentRasterPtr;
        
        FeederConstRasterInfo();
    };    
    
    
    /*!
      \class FeederConstRasterDirectory
      \brief A feeder from an input directory name.
     */
    class TERPEXPORT FeederConstRasterDirectory : public FeederConstRaster
    {
      public :
        
        /*!
          \brief Constructor from an input directory name.
          \param directoryName The directory full path name. 
          \param recursive true if a recursive search must be performed. 
          \param rType The name of the specific driver to instantiate each raster. 
          \param sortFileNames If true, the file names will be sorted.
          \param fileExtensions The file extensions filter (example: ".tif"), or an empty vector if all extensions must be accepted.
        */        
        FeederConstRasterDirectory( const std::string& directoryName,
          const bool recursive,
          const std::string& rType,
          const bool sortFileNames,
          const std::vector< std::string >& fileExtensions );
        
        ~FeederConstRasterDirectory();
        
        //overloads
        te::rst::Raster const* getCurrentObj() const;
        bool moveNext();
        bool moveTo( const unsigned int index );
        void reset();
        unsigned int getObjsCount() const;
        unsigned int getCurrentOffset() const;
        
      protected :
        
        std::string m_rType;
        std::vector< std::string >::size_type m_currentOffset;
        std::vector< std::string > m_filesNames;
        std::auto_ptr< te::rst::Raster > m_currentRasterPtr;
        
        FeederConstRasterDirectory();
    };      
  } // end namespace rp
}   // end namespace te

#endif  // __TERRALIB_RP_INTERNAL_FEEDERSRASTER_H

