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
        
        /*! \enum  Info regarding the uri access (bit field). */
        enum AccessType
        {
          MultipleAccessType = 0, /*!< Allows multiple concurrent connections to the uri. */
          SingleAccessType = 1 /*!< Allows just one single connection to the uri. */
        };        
        
        /*!
         \brief Try to increment the use counter for the given raster URI.
         \param uri RasterURI.
         \param aType Access type.
         \return true if OK, false if the increment could not be done.
         */        
        bool incrementUseCounter( const std::string& uri, const AccessType aType );
        
        /*!
         \brief Decrement the use counter for the given raster URI.
         \param uri RasterURI.
         */                
        void decrementUseCounter( const std::string& uri );

        
      protected :
        
        typedef std::map< std::string, std::pair< AccessType, unsigned long int > > UrisInfoT;
        
        DataSetsManager();
        
        ~DataSetsManager();
        
        mutable UrisInfoT m_openURIS;   //!< Current open URIs.
        
        mutable boost::mutex m_mutex; //!< Internal thread sync mutex.
    };
  } // end namespace gdal
}   // end namespace te

#endif  // __TERRALIB_DATAACCES_GDAL_INTERNAL_DATASETSMANAGER_H
