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

  \brief This class defines functions used to load and save gpm's using GAL and GWT formats,
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

#ifndef __TERRALIB_SA_INTERNAL_SPATIALWEIGHTSEXCHANGER_H
#define __TERRALIB_SA_INTERNAL_SPATIALWEIGHTSEXCHANGER_H

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

  namespace sa
  {
    // Forward declarations
    class GeneralizedProximityMatrix;

    /*!
      \class SpatialWeightsExchanger

      \brief This class defines functions used to load and save gpm's using GAL and GWT formats,
              both formats use a ' ' as separator.

    */
    class TESAEXPORT SpatialWeightsExchanger
    {
      public:

        /*! \brief Default constructor. */
        SpatialWeightsExchanger();

        /*! \brief Virtual destructor. */
        ~SpatialWeightsExchanger();

      public:

        /*!
          \brief Function used to export a gpm to Spatial Weights File GAL Format

          \param gpm            Pointer to a valid graph
          \param pathFileName   Path and file name (with extension) for file creation.

        */
        void exportToGAL(te::sa::GeneralizedProximityMatrix* gpm, std::string pathFileName);

        /*!
          \brief Function used to import a gpm from a Spatial Weights File GAL Format

          \param pathFileName   Path and file name (with extension) with Spatial Weights information.
          \param ds             Pointer to a datasource in case the gal file has the shapefile information.

          \return A Valid gpm pointer if the file was correctly readed and null pointer in other case.

        */
        te::sa::GeneralizedProximityMatrix* importFromGAL(std::string pathFileName, te::da::DataSource* ds = 0);

        /*!
          \brief Function used to export a gpm to Spatial Weights File GWT Format

          \param gpm            Pointer to a valid graph
          \param pathFileName   Path and file name (with extension) for file creation.
          \param distAttrIdx    Edge Atrribute index with distance information

        */
        void exportToGWT(te::sa::GeneralizedProximityMatrix* gpm, std::string pathFileName, int distAttrIdx);

        /*!
          \brief Function used to import a gpm from a Spatial Weights File GWT Format

          \param pathFileName   Path and file name (with extension) with Spatial Weights information.
          \param ds             Pointer to a datasource in case the gwt file has the shapefile information.

          \return A Valid gpm pointer if the file was correctly readed and null pointer in other case.

        */
        te::sa::GeneralizedProximityMatrix* importFromGWT(std::string pathFileName, te::da::DataSource* ds = 0);

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

        */
        void associateGeometry(te::sa::GeneralizedProximityMatrix* gpm, te::da::DataSource* ds);

      private:

        int m_edgeId;  //!< Attribute used as a index counter for edge objects
    };

  } // end namespace sa
} // end namespace te

#endif //__TERRALIB_SA_INTERNAL_SPATIALWEIGHTSEXCHANGER_H