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
  \file terralib/graph/FlowGraphBuilder.h

  \brief This class defines the Flow strategy to build a graph.

  This function needs a vectorial data and table with flow information,
  this table must have the origin and destination info about each flow.

*/

#ifndef __TERRALIB_GRAPH_INTERNAL_FLOWGRAPHBUILDER_H
#define __TERRALIB_GRAPH_INTERNAL_FLOWGRAPHBUILDER_H

// Terralib Includes
#include "../Config.h"
#include "AbstractGraphBuilder.h"

// STL Includes
#include <map>
#include <memory>
#include <vector>

// Boost
#include <boost/ptr_container/ptr_vector.hpp>


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
      \class FlowGraphBuilder

      \brief This class defines the Flow strategy to build a graph.
             
             This function needs a vectorial data and table with flow information,
             this table must have the origin and destination info about each flow.

      \note For now the the shapefile must have only polygon geometries and the
            flow table must be a CSV file.

      \sa AbstractGraphBuilder
    */

    class TEGRAPHEXPORT FlowGraphBuilder : public AbstractGraphBuilder
    {
      public:

        /*! \brief Default constructor. */
        FlowGraphBuilder();

        /*! \brief Virtual destructor. */
        virtual ~FlowGraphBuilder();

        
        /** @name Methods
         *  Methods used by this builder
         */
        //@{


        /*!
          \brief Function used to build the output graph based on input parameters.

          \param shapeFileName  File name wiht vectorial data
          \param linkColumn     Column name from vectorial data used as link column
          \param srid           Vectorial projection id
          \param csvFileName    CSV File name with flow data
          \param fromIdx        Index for column table with origin information.
          \param toIdx          Index for column table with destiny information.
          \param weightIdx      Index for column table with weight information.
          \param dsInfo         Container with data source information
          \param graphType      Attribute used to define the output graph type
          \param gInfo          Container with graph generation parameters

          \return True if the graph was correctly generated and false in other case.

        */
        bool build(const std::string& shapeFileName, const std::string& linkColumn, const int& srid, const std::string& csvFileName, const int& fromIdx, const int& toIdx, const int& weightIdx,
          const std::map<std::string, std::string>& dsInfo, const std::string& graphType, const std::map<std::string, std::string>& gInfo);

        //@}

      protected:

        /*!
          \brief Function used to generated the edge id

          \return Integer value as ID

        */
        int getEdgeId();

        /*!
          \brief Function used to get the data source with the vectorial data

          \return Data source.

        */
        std::auto_ptr<te::da::DataSource> getDataSource(const std::string fileName);

        /*!
          \brief Function used to get the data set with the vectorial data

          \return Data set.

        */
        std::auto_ptr<te::da::DataSet> getDataSet(te::da::DataSource* ds);

        /*!
          \brief Function used to get the data source properties

          \return A vector with all properties from a data source

        */
        boost::ptr_vector<te::dt::Property> getProperties(te::da::DataSource* ds);

        /*!
          \brief Function used to create all vertex object based on vectorial data

          \param shapeFileName    File name wiht vectorial data
          \param linkColumn       Column name from vectorial data used as link column
          \param srid             Vectorial projection id

          \return True if the vertexs was created correctly and false in othe case

        */
        bool createVertexObjects(const std::string& shapeFileName, const std::string& linkColumn, const int& srid);

        /*!
          \brief Function used to create all edges object based on flow table data

          \param csvFileName    CSV File name with flow data
          \param fromIdx        Index for column table with origin information.
          \param toIdx          Index for column table with destiny information.
          \param weightIdx      Index for column table with weight information.

          \return True if the edges was created correctly and false in othe case

        */
        bool createEdgeObjects(const std::string& csvFileName, const int& fromIdx, const int& toIdx, const int& weightIdx);

     private:

        int m_edgeId;  //!< Attribute used as a index counter for edge objects
    };
  } // end namespace graph
} // end namespace te

#endif // __TERRALIB_GRAPH_INTERNAL_LDDGRAPHBUILDER_H