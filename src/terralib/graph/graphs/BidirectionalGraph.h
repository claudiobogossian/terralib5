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
  \file BidirectionalGraph.h

  \brief This is a implementation of a Bidirectional Graph.
         By convention a bidirectional graph provides access to 
         out-and in edges.
*/

#ifndef __TERRALIB_GRAPH_INTERNAL_BIDIRECTIONALGRAPH_H
#define __TERRALIB_GRAPH_INTERNAL_BIDIRECTIONALGRAPH_H

// Terralib Includes
#include "../Config.h"
#include "Graph.h"

// STL Includes
#include <vector>

namespace te
{
  namespace graph
  {
    //forward declarations
    class AbstractCachePolicy;
    class AbstractGraphLoaderStrategy;

    /*!
      \class BidirectionalGraph

      \brief This is a implementation of a Bidirectional Graph.
        By convention a bidirectional graph provides access to 
        out-and in edges.

      \sa AbstractGraph, GraphData, GraphCache
    */

    class TEGRAPHEXPORT BidirectionalGraph : public Graph
    {
      public:

        /*! \brief constructor. */
        BidirectionalGraph();

        /*!
          \brief Constructor

          \param metadata   A pointer to a graph metadata implementation

        */
        BidirectionalGraph(GraphMetadata* metadata);

        /*!
          \brief Constructor

          \param cp   A pointer to a cache policy implementation

          \param ls   A pointer to a loader strategy implementation

        */
        BidirectionalGraph(AbstractCachePolicy* cp, AbstractGraphLoaderStrategy* ls);

        /*! \brief Virtual destructor. */
        ~BidirectionalGraph();

        /** @name Vertex Access Methods
         *  Method used to access vertex elements from a graph.
         */
        //@{

        /*!
          \brief The neighborhood of a vertex v is an induced subgraph of the graph, formed by all vertices adjacent to v.

          \param id   The attribute used to identify the vertex element

          \return A vector with vertex elements.
         */
        virtual std::vector<te::graph::Vertex*> getVertexNeighborhood(int id);

        /*!
          \brief This function indicates if a desired element is a isolated vertex.

          \param id The attribute used to identify the vertex element

          \param flag Flag used to indicating if the element is a isolated vertex.

          \return True if the vertex element was found and false in other case
        */
        virtual bool isIsolateVertex(int id, bool& flag);

        /*!
          \brief This function indicates if a desired element is a source vertex.

          \param id   The attribute used to identify the vertex element

          \param flag Flag used to indicating if the element is a source vertex.

          \return  True if the vertex element was found and false in other case
        */
        virtual bool isSourceVertex(int id, bool& flag);

        /*!
          \brief This function indicates if a desired element is a sink vertex.

          \param id   The attribute used to identify the vertex element

          \param flag Flag used to indicating if the element is a sink vertex.

          \return  True if the vertex element was found and false in other case
        */
        virtual bool isSinkVertex(int id, bool& flag);

        //@}

        /** @name Edge Access Methods
         *  Method used to access edge elements from a graph.
         */
        //@{
        
        /*!
          \brief Add a new edge element to a graph

          \param e Edge element

          \note This function turns the dirty flag of current GraphData to true, the
                new flag of the edge turns to true.

         */
        virtual void add(Edge* e);

        /*!
          \brief This function removes the edge element from graph, also was removed
                 in data source.

          \param id Edge identification

         */
        virtual void removeEdge(int id);

        /*!
          \brief It returns all edges that came in a vertex

          \param vId  The attribute used to identify the vertex element

          \return A vector with edge elements.
        */
        virtual std::vector<te::graph::Edge*> getInEdges(int vId);

        /*!
          \brief It returns all edges that came out a vertex

          \param vId  The attribute used to identify the vertex element

          \return A vector with edge elements.
        */
        virtual std::vector<te::graph::Edge*> getOutEdges(int vId);

        //@}

    };

  } // end namespace graph
} // end namespace te

#endif // __TERRALIB_GRAPH_INTERNAL_BIDIRECTIONALGRAPH_H