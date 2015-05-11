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
  \file AbstractGraphLoaderStrategy.h

  \brief This class define the main functions necessary to
        save and load the graph data and metadata information
        using the Graph Data and Graph Cache conceptions.
*/

#ifndef __TERRALIB_GRAPH_INTERNAL_ABSTRACTGRAPHLOADERSTRATEGY_H
#define __TERRALIB_GRAPH_INTERNAL_ABSTRACTGRAPHLOADERSTRATEGY_H

// Terralib Includes
#include "../core/GraphMetadata.h"
#include "../Config.h"

namespace te
{
  namespace graph
  {
    class AbstractGraph;
    class Edge;
    class GraphCache;
    class GraphData;
    class GraphMetadata;
    class Vertex;
    /*!
      \class AbstractGraphLoaderStrategy

      \brief This class define the main functions necessary to
        save and load the graph data and metadata information
        using the Graph Cache conception.

      \sa AbstractGraph, GraphMetadata, GraphCache
    */

    class TEGRAPHEXPORT AbstractGraphLoaderStrategy
    {
      public:

        /*! \brief Default constructor. */
        AbstractGraphLoaderStrategy(te::graph::GraphMetadata* metadata);

        /*! \brief Default destructor. */
        virtual ~AbstractGraphLoaderStrategy();

         /** @name Abstract Graph Loader Strategy Methods
        *  Method used to manager a graph data
        */
        //@{

        /*!
        \brief Functio used to load a group of vertex elements given a base element

        \param vertexId   Attribute used to identify the base element

        \param g          Pointer to a graph, parent of this element

        \param gc         This is a optional attribute, if present the cache will be check if the already been loaded

        */
        virtual void loadDataByVertexId(int vertexId, te::graph::AbstractGraph* g, te::graph::GraphCache* gc = 0) = 0;

        /*!
        \brief Functio used to load a group of edges elements given a base element

        \param edgeId     Attribute used to identify the base element

        \param g          Pointer to a graph, parent of this element

        \param gc         This is a optional attribute, if present the cache will be check if the already been loaded

        */
        virtual void loadDataByEdgeId(int edgeId, te::graph::AbstractGraph* g, te::graph::GraphCache* gc = 0) = 0;

        /*!
        \brief Save the graph data structure in Data Source

        \param data   Pointer to a valid graph data

        */
        virtual void saveData(GraphData* data);

        /*!
        \brief Function used to remove a edge saved in a data source

        \param id The edge identifier

        */
        virtual void removeEdge(int id);

        /*!
        \brief Function used to remove a vertex saved in a data source

        \param id The vertex identifier

        */
        virtual void removeVertex(int id);

        /*!
          \brief It returns a pointer to a class that describes the graph metadata

          \return A valid pointer if exists and null pointer in other case
        */
        te::graph::GraphMetadata* getMetadata();

      protected:

        /*!
          \brief Used to save the edge elements from a graph data

          \param data   Pointer to a valid graph data
        */
        void saveGraphEdgeList(GraphData* data);

        /*!
          \brief Used to save the vertex elements attributes from a graph data

          \param data   Pointer to a valid graph data
        */
        void saveVertexAttributes(GraphData* data);

        /*!
          \brief Used to save the vertex elements from a graph data

          \param data   Pointer to a valid graph data
        */
        void saveGraphVertexList(GraphData* data);

        /*!
          \brief Used to save the edges elements attributes from a graph data

          \param data   Pointer to a valid graph data
        */
        void saveEdgeAttributes(GraphData* data);

        /*!
          \brief Function used to load one vertex given a ID

          \param id  The vertex identifier

          \note This function is used in case the graph was saved using the vertex list

          \return  A valid vertex pointer if the element was found and a null pointer in other case
        */
        Vertex* loadVertex(int id);

        /*!
          \brief Function used to load one vertex given a ID

          \param id  The vertex identifier

          \note This function is used in case the graph was saved using the edge list

          \return  A valid vertex pointer if the element was found and a null pointer in other case
        */
        Vertex* loadVertexAttrs(int id);

        /*!
          \brief Function used to load one edge given a ID

          \param id  The edge identifier

          \note This function is used in case the graph was saved using the edge list

          \return  A valid edge pointer if the element was found and a null pointer in other case
        */
        Edge* loadEdge(int id);

        /*!
          \brief Function used to load one edge given a ID

          \param id  The edge identifier

          \note This function is used in case the graph was saved using the vertex list

          \return  A valid vertex pointer if the element was found and a null pointer in other case
        */
        Edge* loadEdgeAttrs(int id);

        //@}

       protected:

        te::graph::GraphMetadata* m_graphMetadata; //!< Graph metadata attribute
    };
  } // end namespace graph
} // end namespace te

#endif // __TERRALIB_GRAPH_INTERNAL_ABSTRACTGRAPHLOADERSTRATEGY_H