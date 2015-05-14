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
  \file GraphDataManager.h

  \brief This class defines a interface to access the graph
         elements inside a data source. Its use a implementation of
         Loader Strategy that defines how a data must be loaded.
         
*/

#ifndef __TERRALIB_GRAPH_INTERNAL_GRAPHDATAMANAGER_H
#define __TERRALIB_GRAPH_INTERNAL_GRAPHDATAMANAGER_H

// Terralib Includes
#include "../Config.h"
#include "../Enums.h"

namespace te
{
  namespace graph
  {
    //forward declarations
    class AbstractGraph;
    class AbstractGraphLoaderStrategy;
    class Edge;
    class GraphCache;
    class GraphData;
    class Vertex;

    /*!
      \class AGraphDataManager

      \brief This class defines a interface to access the graph
         elements inside a data source. Its use a implementation of
         Loader Strategy that defines how a data must be loaded.

      \sa AbstractGraph, AbstractGraphLoaderStrategy
    */

    class TEGRAPHEXPORT GraphDataManager
    {
      public:

        /*! \brief Default constructor. */
        GraphDataManager(te::graph::AbstractGraph* g);

        /*! \brief Default destructor. */
        virtual ~GraphDataManager();


        /*!
          \brief Function used to get the current loader strategy

          \return A valid pointer to a loader strategy if exist and a null pointer in other case

        */
        AbstractGraphLoaderStrategy* getLoaderStrategy();

        /*!
          \brief Function used to set a current loader strategy

          \param loaderStrategy A pointer to loader strategy implementation

        */
        void setLoaderStrategy(AbstractGraphLoaderStrategy* loaderStrategy);

         /** @name Graph Data Manager Methods
        *  Method used to manager a graph data
        */
        //@{

        /*!
          \brief Load a set of vertex elements that includes the desired element.

          \param vertexId The vertex identifier of the desired element.

          \param gc A Graph Cache used to verify if the element already in memory

        */
        void loadGraphDataByVertexId(int vertexId, te::graph::GraphCache* gc = 0);

        /*!
          \brief Load a set of edges elements that includes the desired element.

          \param edgeId The edge identifier of the desired element.

          \param gc A Graph Cache used to verify if the element already in memory

        */
        void loadGraphDataByEdgeId(int edgeId, te::graph::GraphCache* gc = 0);

        /*!
          \brief Save the graph data structure in Data Source

          \param data A graph data with a group of vertex and edge elements

          \note Only the elements with a flag new with true value will be saved and
                the elements with the flag dirty with true value will be updated the
                other elements will be ignored

        */
        void saveGraphData(GraphData* data);

        /*!
          \brief Function used to remove a edge from data source

          \param id The edge Id

        */
        void removeEdge(int id);

        /*!
          \brief Function used to remove a vertex from data source

          \param id The vertex Id

        */
        void removeVertex(int id);

        //@}

      private:

        AbstractGraphLoaderStrategy* m_loadStrategy;    //!< Pointer to a loader strategy object
        AbstractGraph* m_graph;                         //!< Pointer to a graph object
    };
  } // end namespace graph
} // end namespace te

#endif // __TERRALIB_GRAPH_INTERNAL_GRAPHDATAMANAGER_H