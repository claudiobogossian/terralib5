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
  \file DataLoader.h

  \brief This file contains a class called DataLoader
*/

#ifndef __TERRALIB_ST_INTERNAL_DATALOADER_H
#define __TERRALIB_ST_INTERNAL_DATALOADER_H

// STLoader
#include "Config.h"

// STL
#include "vector"

// Forward declarations
namespace te { namespace da { class DataSet; class DataSource;} }

namespace te { namespace st {  class ObservationSet;  class MovingObject;  class Coverage; class LocatedTimeSeries; } }

namespace te
{
  namespace stloader
  {
    // Forward declarations
    class ObservationDataSetMetadata;
    class MovingObjectDataSetMetadata;
    class LocatedTimeSeriesDataSetMetadata;

     /*!
      \class DataLoader

      \brief A class that contains a set of static functions able to load 
      spatiotemporal data from DataSet and DataSource.

      \sa DataSet DataSource
    */
    class TESTLOADEREXPORT DataLoader
    {
      public:
        
         /*! 
          \brief It loads observations from a DataSet. 
          
          \param ds         A pointer to a data set whose observations will be loaded.
          \param metadata   The metadata about the data set observations. 
          \param output     The returned observation set from the data set.

          \note The DataSet internal cursor must be pointed to a valid position.
          \note It will NOT take the ownership of the input pointers.
        */
        static void loadObservationSet(te::da::DataSet* ds, 
                        ObservationDataSetMetadata& metadata, 
                        te::st::ObservationSet*& output);

        /*! 
          \brief It loads moving objects from a DataSet. 
          
          \param ds         A pointer to a data set whose moving objects will be loaded.
          \param metadata   The metadata about the data set moving objects.
          \param output     The returned moving objects from the data set.

          \note The DataSet internal cursor must be pointed to a valid position.
          \note It will NOT take the ownership of the input pointers.
        */
        static void loadMovingObjects(te::da::DataSet* ds, 
                        MovingObjectDataSetMetadata& metadata, 
                        std::vector<te::st::MovingObject*>& output);

        /*! 
          \brief It loads moving objects from a KML file through a OGR DataSource. 
          
          \param ds         A pointer to a OGR data source whose moving objects will be loaded.
          \param metadata   The XML file name that contains the metadata about KML moving objects. 
          \param output     The returned moving objects from the data source.

          \note The DataSource pointer will be open internally. 
          \note It will NOT take the ownership of the input pointers.
          \note The caller will take the ownership of the output moving object pointers.
        */
        static void loadMovingObjects(te::da::DataSource* ds, 
                        const std::string& metadata, 
                        std::vector<te::st::MovingObject*>& output);

        /*! 
          \brief It loads a coverage from a set of raster files through a GDAL DataSource. 
          
          \param ds         A pointer to a GDAL data source whose coverage will be loaded.
          \param metadata   The XML file name that contains the metadata about raster coverage. 
          \param output     The returned coverage from the data source.

          \note The DataSource pointer will be open internally. 
          \note It will NOT take the ownership of the data source pointer.
          \note The coverage pointer will be created internally and the caller will tabke its ownership.
        */
        static void loadCoverage(te::da::DataSource* ds, const std::string& metadata, 
                        te::st::Coverage** output);

        /*! 
          \brief It loads located time series from a DataSet. 
          
          \param ds         A pointer to a data set whose located time series will be loaded.
          \param metadata   The metadata about the data set located time series.
          \param output     The returned located time series from the data set.

          \note The DataSet internal cursor must be pointed to a valid position.
        */
        static void loadLocatedTimeSeries(te::da::DataSet* ds, 
                        LocatedTimeSeriesDataSetMetadata& metadata, 
                        std::vector<te::st::LocatedTimeSeries*>& output);
       
    };
  } // end namespace stloader
}   // end namespace te

#endif  // __TERRALIB_STLOADER_INTERNAL_DATALOADER_H

