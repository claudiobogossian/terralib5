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
 \file terralib/dataaccess/gdal/DataSetUseCounter.h
 
 \brief GDAL data set use counter.
 */

#ifndef __TERRALIB_DATAACCES_GDAL_INTERNAL_DATASETUSECOUNTER_H
#define __TERRALIB_DATAACCES_GDAL_INTERNAL_DATASETUSECOUNTER_H

#include "Config.h"
#include "Exception.h"

#include <boost/noncopyable.hpp>

#include <string>

namespace te
{
  namespace gdal
  {
    /*!
     \class DataSetUseCounter
     
     \brief GDAL data set use counter.
     */
    class TEGDALEXPORT DataSetUseCounter : protected boost::noncopyable
    {
      public :
        
        /*!
         \brief Default constructor.
         \param uri Raster URI.
         \exception Exception It can throw an exception (code te::common::NO_CONNECTION_AVAILABLE ) if:
                     <ul>
                     <li>the maximum number of simultaneous accesses has been reached</li>
                     </ul>     
         */        
        DataSetUseCounter( const std::string& uri ) throw( te::gdal::Exception );
        
        ~DataSetUseCounter();
        
      private :
        
        std::string m_uri; //!< Data set uri.
    };
  } // end namespace gdal
}   // end namespace te

#endif  // __TERRALIB_DATAACCES_GDAL_INTERNAL_DATASETSMANAGER_H
