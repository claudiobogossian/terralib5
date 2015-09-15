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
  \file RAGGraphBuilder.h

  \brief This class defines the RAG strategy to build a graph,

        This is strategy is based on adjacency of a region.

*/

#ifndef __TERRALIB_GRAPH_INTERNAL_RAGGRAPHBUILDER_H
#define __TERRALIB_GRAPH_INTERNAL_RAGGRAPHBUILDER_H

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
      \class RAGGraphBuilder

      \brief This class defines the RAG strategy to build a graph,

             This is strategy is based on adjacency of a region.

      \sa AbstractGraphBuilder
    */

    class TEGRAPHEXPORT RAGGraphBuilder : public AbstractGraphBuilder
    {
      public:

        /*! \brief Default constructor. */
        RAGGraphBuilder();

        /*! \brief Virtual destructor. */
        virtual ~RAGGraphBuilder();

        
        /** @name Methods
         *  Methods used by this builder
         */
        //@{


        /*!
          \brief Function used to generated a graph using the RAG strategy

          \param shapeFileName  File name wiht vectorial data
          \param linkColumn     Column name from vectorial data used as link column
          \param srid           Vectorial projection id
          \param dsInfo         Container with data source information
          \param graphType      Attribute used to define the output graph type
          \param gInfo          Container with graph generation parameters

          \return True if the graph was correctly generated and false in other case.

         */
         bool build(const std::string& shapeFileName, const std::string& linkColumn, const int& srid, 
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
        std::auto_ptr<te::da::DataSource> getDataSource(const std::string& fileName);

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
          \brief Function used to create all edges object based on vectorial data

          \param shapeFileName    File name wiht vectorial data
          \param linkColumn       Column name from vectorial data used as link column

          \return True if the edges was created correctly and false in othe case

        */
        bool createEdgeObjects(const std::string& shapeFileName, const std::string& linkColumn);

        /*!
          \brief Function used to get the geometry column name

          \param shapeFileName    File name wiht vectorial data
          \param columnName       Used to set the geometry column name

          \return True if the the vectorial data was a geometry column and false in othe case

        */
        bool getGeometryColumn(const std::string& shapeFileName, std::string& columnName);

      private:

        int m_edgeId;     //!< Attribute used as a index counter for edge objects

    };

  } // end namespace graph
} // end namespace te

#endif // __TERRALIB_GRAPH_INTERNAL_RAGGRAPHBUILDER_H