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
  \file GPMGraphBuilder.h

  \brief This class defines the GPM strategy to build a graph,

        This is builder uses diferent strategies to build a graph based
        on a generalized proximity matrix.

*/

#ifndef __TERRALIB_GRAPH_INTERNAL_GPMGRAPHBUILDER_H
#define __TERRALIB_GRAPH_INTERNAL_GPMGRAPHBUILDER_H

// Terralib Includes
#include "../Config.h"
#include "AbstractGraphBuilder.h"

// STL Includes
#include <vector>
#include <map>

namespace te
{
  // Forward declarations
  namespace da 
  { 
    class DataSource;
    class DataSet;
  }

  namespace dt { class Property; }

  namespace gm { class GeometryProperty; }

  namespace graph
  {
  
    /*!
      \class GPMGraphBuilder

      \brief This class defines the GPM strategy to build a graph,

        This is builder uses diferent strategies to build a graph based
        on a generalized proximity matrix.

      \sa AbstractGraphBuilder
    */

    class TEGRAPHEXPORT GPMGraphBuilder : public AbstractGraphBuilder
    {
      public:

        /*! \brief Default constructor. */
        GPMGraphBuilder();

        /*! \brief Virtual destructor. */
        virtual ~GPMGraphBuilder();

        
        /** @name Methods
         *  Methods used by this builder
         */
        //@{


        /*!
          \brief Function used to create a empty graph

          \param dsInfo         Container with data source information
          \param graphType      Attribute used to define the output graph type
          \param gInfo          Container with graph generation parameters

          \return True if the graph was correctly generated and false in other case.

        */
        bool setGraphInfo(const std::map<std::string, std::string>& dsInfo, const std::string& graphType, const std::map<std::string, std::string>& gInfo);

        /*!
          \brief Function used to generated a graph using the GPM Adjacency Strategy

          \param ds             File name wiht vectorial data
          \param dataSetName    Data Set name that contains the geometry information
          \param columnId       Column id from vectorial data used as link column
          \param calcDist       Flag used to indicate if has to create the distance attribute

          \return True if the graph was correctly generated and false in other case.

         */
        bool buildAdjacency(std::auto_ptr<te::da::DataSource> ds, std::string dataSetName, std::string columnId, bool calcDist);

        /*!
          \brief Function used to generated a graph using the GPM Distance Strategy

          \param ds             File name wiht vectorial data
          \param dataSetName    Data Set name that contains the geometry information
          \param columnId       Column id from vectorial data used as link column
          \param dist           Value with distance information.

          \return True if the graph was correctly generated and false in other case.

         */
        bool buildDistance(std::auto_ptr<te::da::DataSource> ds, std::string dataSetName, std::string columnId, double dist);

    protected:

        /*!
          \brief Function used to create all vertex object based on data set

          \param dataSet      Valid pointer to a dataset
          \param columnId     Column name from vectorial data used as link column
          \param srid         Vectorial projection id

        */
        void createVertexObjects(te::da::DataSet* dataSet, std::string columnId, int srid);

        /*!
          \brief Function used to create all edges object based on data set, using the adjacency strategy

          \param dataSet      Valid pointer to a dataset
          \param columnId     Column name from vectorial data used as link column
          \param calcDist     Flag used to indicate if has to create the distance attribute

        */
        void createAdjacencyEdges(te::da::DataSet* dataSet, std::string columnId, bool calcDist);

        /*!
          \brief Function used to create all edges object based on data set, using the distance strategy

          \param dataSet      Valid pointer to a dataset
          \param columnId     Column name from vectorial data used as link column
          \param distance     Value with distance information.

        */
        void createDistanceEdges(te::da::DataSet* dataSet, std::string columnId, double distance);

        /*!
          \brief Function used to generated the edge id

          \return Integer value as ID

        */
        int getEdgeId();

      //@}

      private:

        int m_edgeId;     //!< Attribute used as a index counter for edge objects

    };

  } // end namespace graph
} // end namespace te

#endif // __TERRALIB_GRAPH_INTERNAL_RAGGRAPHBUILDER_H