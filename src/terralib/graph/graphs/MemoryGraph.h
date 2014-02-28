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
  \file MemoryGraph.h

  \brief  This is the memory graph implementation and do not uses a
          cache policy and a graph loader.
*/

#ifndef __TERRALIB_GRAPH_INTERNAL_MEMORYGRAPH_H
#define __TERRALIB_GRAPH_INTERNAL_MEMORYGRAPH_H

// Terralib Includes
#include "../core/AbstractGraph.h"
#include "../Config.h"


// STL Includes
#include <vector>

namespace te
{
  namespace dt { class Property; }

  namespace graph
  {
    //forward declarations
    class Edge;
    class GraphMetadata;
    class Vertex;
    
    
    /*!
      \class Graph

      \brief  This is the memory graph implementation and do not uses a
          cache policy and a graph loader.

      \sa AbstractGraph
    */

    class TEGRAPHEXPORT MemoryGraph : public AbstractGraph
    {
      public:

        /*! \brief constructor. */
        MemoryGraph();

      /*!
        \brief Constructor

        \param cp   A pointer to a cache policy implementation

        \param ls   A pointer to a loader strategy implementation

        */
        MemoryGraph(GraphMetadata* metadata);

        /*! \brief Virtual destructor. */
        ~MemoryGraph();

        typedef std::map<int, Vertex*> VertexMap; //!< typedef for vertex map

        typedef std::map<int, Edge*> EdgeMap;     //!< typedef for edge map

        
        /** @name Vertex Access Methods
         *  Method used to access vertex elements from a graph.
         */
        //@{

        /*!
          \brief Add a new vertex element to a graph

          \param v Vertex element

          \note This function turns the dirty flag of current GraphData to true, the
                new flag of the vertex turns to true.

         */
        virtual void add(Vertex*  v);

        /*!
          \brief Update the vertex element

          \param v Vertex element

          \note This function turns the dirty flag of current GraphData to true and
                also the dirty flag of the vertex.

         */
        virtual void update(Vertex*  v);

        /*!
          \brief This function removes the vertex element from graph, also was removed
                 in data source.

          \param id Vertex identification

         */
        virtual void removeVertex(int id);

        /*!
          \brief It returns the vertex element if it's exist.

          \param id Vertex identification

          \return A valid vertex point if the element was found and a null pointer in other case.
        */
        virtual te::graph::Vertex* getVertex(int id);


         /*!
          \brief Add a new property associated to the vertex element

          param p  New property to be associated with vertex elements.

          \note It's important before using this function call the flush() function, its necessary
                to force the memory clear and the elements will be loaded with the right size of
                properties.
        */
        virtual void addVertexProperty(te::dt::Property* p);


        /*!
          \brief Remove a property associated to the vertex element

          \param idx Index of the property 
        */
        virtual void removeVertexProperty(int idx);

        /*!
          \brief Get a vertex property given a index

          \param idx Index of the property 

          \return A property associated to the vertex element if the index is right and a null pointer in other case.

        */
        virtual te::dt::Property* getVertexProperty(int idx);

        /*!
          \brief Used to verify the number of properties associated to vertex elements

          \return  Integer value with the number of properties.

        */
        virtual int getVertexPropertySize();


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
          \brief Update the edge element

          \param e Edge element

          \note This function turns the dirty flag of current GraphData to true and
                also the dirty flag of the edge.

         */
        virtual void update(Edge* e);

        /*!
          \brief This function removes the edge element from graph, also was removed
                 in data source.

          \param id Edge identification

         */
        virtual void removeEdge(int id);

        /*!
          \brief It returns the edge element if it's exist.

          \param id Vertex identification

          \return A valid vertex point if the element was found and a null pointer in other case.
        */
        virtual te::graph::Edge* getEdge(int id);

         /*!
          \brief Add a new property associated to the edge element

          param p  New property to be associated with edge elements.

          \note It's important before using this function call the flush() function, its necessary
                to force the memory clear and the elements will be loaded with the right size of
                properties.
        */
        virtual void addEdgeProperty(te::dt::Property* p);

        /*!
          \brief Remove a property associated to the edge element

          \param idx Index of the property 
        */
        virtual void removeEdgeProperty(int idx);

        /*!
          \brief Get a edge property given a index

          \param idx Index of the property 

          \return A property associated to the edge element if the index is right and a null pointer in other case.

        */
        virtual te::dt::Property* getEdgeProperty(int idx);

        /*!
          \brief Used to verify the number of properties associated to edge elements

          \return  Integer value with the number of properties.

        */
        virtual int getEdgePropertySize();

        //@}

        /*!
          \brief Function used to access the graph metadata

          \return A pointer to a class that defines the graph metadata

        */
        virtual te::graph::GraphMetadata* getMetadata();

        /*!
          \brief Function used to clear the memory cache, all elements was released from
                 memory, if any element was changes it will be saved.

          \return 

        */
        virtual void flush();

        /*!
        \brief It returns the the vertex map

        \return Map with all vertex from this graph data
        */
        VertexMap& getVertexMap();

        /*!
        \brief It returns the the edge map

        \return Map with all edges from this graph data
        */
        EdgeMap& getEdgeMap();


      protected:

        GraphMetadata* m_metadata;        //!< Graph Data loader strategy

        VertexMap       m_vertexMap;      //!< This map contains all vertexs from this graph.
        EdgeMap         m_edgeMap;        //!< This map contains all edges from this graph.
    };

  } // end namespace graph
} // end namespace te

#endif // __TERRALIB_GRAPH_INTERNAL_MEMORYGRAPH_H
