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
 \file terralib/dataaccess/gdal/DataSetsManager.h
 
 \brief GDAL data set manager.
 */

#ifndef __TERRALIB_DATAACCES_GDAL_INTERNAL_DATASETSMANAGER_H
#define __TERRALIB_DATAACCES_GDAL_INTERNAL_DATASETSMANAGER_H

#include "Config.h"
#include "../common/Singleton.h"

#include <map>
#include <string>

#include <boost/thread/mutex.hpp>

#include <memory>

namespace te
{
  namespace gdal
  {
    class DataSet;
    
    /*!
     \class DataSetsManager
     
     \brief GDAL data set manager.
     */
    class TEGDALEXPORT DataSetsManager : public te::common::Singleton< te::gdal::DataSetsManager >
    {
      friend class te::common::Singleton< te::gdal::DataSetsManager >;
      
      public :
        
        /*!
         \brief Try to increment the use counter for the given raster URI.
         \param uri RasterURI.
         \return true if OK, false if the increment could not be done.
         */        
        bool incrementUseCounter( const std::string& uri );
        
        /*!
         \brief Decrement the use counter for the given raster URI.
         \param uri RasterURI.
         */                
        void decrementUseCounter( const std::string& uri );

        /*!
         \brief Return the use counter for the given raster URI.
         \param uri RasterURI.
         \return The use counter for the given raster URI.
         */        
        unsigned int getUseCounter( const std::string& uri ) const;

        /*!
         \brief Set the maximum use counter for all URIS.
         \param counter The new maximum use counter value.
         */       
        void setMaxUseCounter( const unsigned int counter );
        
        /*!
         \brief Return true if the use counter for the given raster URI has reached its maximum allowed value.
         \return true if the use counter for the given raster URI has reached its maximum allowed value.
         */  
        bool hasReachedMaxUseCounter( const std::string& uri ) const;
        
      protected :
        
        DataSetsManager();
        
        ~DataSetsManager();
        
        unsigned int m_maxUseCounter; //!< Maximum allowed use couter for all URIs.
      
        mutable std::map< std::string, unsigned long int > m_openURIS;   //!< Current open URIs.
        
        mutable boost::mutex m_mutex; //!< Internal thread sync mutex.
    };
  } // end namespace gdal
}   // end namespace te

#endif  // __TERRALIB_DATAACCES_GDAL_INTERNAL_DATASETSMANAGER_H
