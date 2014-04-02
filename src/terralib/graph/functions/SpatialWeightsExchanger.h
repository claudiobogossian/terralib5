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
  \file SpatialWeightsExchanger.h

  \brief This class defines functions used to load and save graphs using GAL and GWT formats,
  both formats use a ' ' as separator.

  GAL FORMAT

  0   NUMBER_OBSERVATIONS   DATASET_NAME  ATTRIBUTE_ID_NAME  (HEADER LINE)
  OBSERVATION_ID  NUMBER_NEIGHBOURS
  NEIGHBOURS_1  NEIGHBOURS_2 ... NEIGHBOURS_N
  OBSERVATION_ID  NUMBER_NEIGHBOURS
  NEIGHBOURS_1  NEIGHBOURS_2 ... NEIGHBOURS_N
  ...


  GWT FORMAT
  
  0  NUMBER_OBSERVATIONS  DATASET_NAME  ATTRIBUTE_ID_NAME  (HEADER LINE)
  OBSERVATION_ID_FROM  OBSERVATION_ID_TO  DISTANCE
  OBSERVATION_ID_FROM  OBSERVATION_ID_TO  DISTANCE
  OBSERVATION_ID_FROM  OBSERVATION_ID_TO  DISTANCE
  ...
*/

#ifndef __TERRALIB_GRAPH_INTERNAL_SPATIALWEIGHTSEXCHANGER_H
#define __TERRALIB_GRAPH_INTERNAL_SPATIALWEIGHTSEXCHANGER_H

// Terralib Includes
#include "../Config.h"

//STL Includes
#include <map>
#include <string>

namespace te
{
  namespace da
  {
    class DataSource;
  }

  namespace graph
  {
    //forward declarations
    class AbstractGraph;

    /*!
      \class SpatialWeightsExchanger

      \brief This class defines functions used to load and save graphs using GAL and GWT formats,
              both formats use a ' ' as separator.

    */
    class TEGRAPHEXPORT SpatialWeightsExchanger
    {
      public:

        /*! \brief Default constructor. */
        SpatialWeightsExchanger();

        /*! \brief Virtual destructor. */
        ~SpatialWeightsExchanger();

      public:

        /*!
          \brief Function used to export a graph to Spatial Weights File GAL Format

          \param g              Pointer to a valid graph
          \param pathFileName   Path and file name (with extension) for file creation.
          \param dataSetName    Data Set name that contains the geometry information
          \param propertyName   The attribute name used as vertex id and dataset id column

        */
        void exportToGAL(te::graph::AbstractGraph* g, std::string pathFileName, std::string dataSetName = "", std::string propertyName = "");

        /*!
          \brief Function used to import a graph from a Spatial Weights File GAL Format

          \param pathFileName   Path and file name (with extension) with Spatial Weights information.
          \param dsInfo         Container with data source information
          \param graphType      Attribute used to define the output graph type
          \param gInfo          Container with graph generation parameters
          \param ds             Pointer to a datasource in case the gal file has the shapefile information.

          \return A Valid graph pointer if the file was correctly readed and null pointer in other case.

        */
        te::graph::AbstractGraph* importFromGAL(std::string pathFileName, std::map<std::string, std::string> dsInfo, std::string graphType, 
                                                std::map<std::string, std::string> gInfo, te::da::DataSource* ds = 0);

        /*!
          \brief Function used to export a graph to Spatial Weights File GWT Format

          \param g              Pointer to a valid graph
          \param pathFileName   Path and file name (with extension) for file creation.
          \param dataSetName    Data Set name that contains the geometry information
          \param propertyName   The attribute name used as vertex id and dataset id column

        */
        void exportToGWT(te::graph::AbstractGraph* g, std::string pathFileName, int distAttrIdx, std::string dataSetName = "", std::string propertyName = "");

        /*!
          \brief Function used to import a graph from a Spatial Weights File GWT Format

          \param pathFileName   Path and file name (with extension) with Spatial Weights information.
          \param dsInfo         Container with data source information
          \param graphType      Attribute used to define the output graph type
          \param gInfo          Container with graph generation parameters
          \param ds             Pointer to a datasource in case the gwt file has the shapefile information.

          \return A Valid graph pointer if the file was correctly readed and null pointer in other case.

        */
        te::graph::AbstractGraph* importFromGWT(std::string pathFileName, std::map<std::string, std::string> dsInfo, std::string graphType, 
                                                std::map<std::string, std::string> gInfo, te::da::DataSource* ds = 0);

      protected:

        /*!
          \brief Function used to generated the edge id

          \return Integer value as ID

        */
        int getEdgeId();

        /*!
          \brief Function used to associate the geometry coord attribute to vertex objects.

          \param g              Pointer to a valid graph
          \param ds             Pointer to a valid data source
          \param dataSetName    Data Set name that contains the geometry information
          \param propertyName   The attribute name used as vertex id and dataset id column

        */
        void associateGeometry(te::graph::AbstractGraph* g, te::da::DataSource* ds, std::string dataSetName, std::string propertyName);

      private:

        int m_edgeId;  //!< Attribute used as a index counter for edge objects
    };

  } // end namespace graph
} // end namespace te

#endif //__TERRALIB_GRAPH_INTERNAL_SPATIALWEIGHTSEXCHANGER_H