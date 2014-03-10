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
  \file AbstractGraph.h

  \brief  Abstract class used to define the main functions of graph struct. All
    graph implementations must used this class. 
*/

#ifndef __TERRALIB_GRAPH_INTERNAL_ABSTRACTGRAPH_H
#define __TERRALIB_GRAPH_INTERNAL_ABSTRACTGRAPH_H

// Terralib Includes
#include "../Config.h"

// STL Includes
#include <vector>

namespace te
{
  namespace dt { class Property; }

  namespace graph
  {
    //forward declarations
    class GraphMetadata;
    class Edge;
    class Vertex;
    
    /*!
      \class AbstractGraph

      \brief Abstract class used to define the main functions of graph struct. All
             graph implementations must used this class. 

    */

    class TEGRAPHEXPORT AbstractGraph
    {
      public:

        /*! \brief Default constructor. */
        AbstractGraph();

        /*! \brief Virtual destructor. */
        virtual ~AbstractGraph();

        
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
        virtual void add(Vertex*  v) = 0;

        /*!
          \brief Update the vertex element

          \param v Vertex element

          \note This function turns the dirty flag of current GraphData to true and
                also the dirty flag of the vertex.

         */
        virtual void update(Vertex*  v) = 0;

        /*!
          \brief This function removes the vertex element from graph, also was removed
                 in data source.

          \param id Vertex identification

         */
        virtual void removeVertex(int id) = 0;

        /*!
          \brief It returns the vertex element if it's exist.

          \param id Vertex identification

          \return A valid vertex point if the element was found and a null pointer in other case.
        */
        virtual te::graph::Vertex* getVertex(int id) = 0;

         /*!
          \brief Add a new property associated to the vertex element

          param p  New property to be associated with vertex elements.

          \note It's important before using this function call the flush() function, its necessary
                to force the memory clear and the elements will be loaded with the right size of
                properties.
        */
        virtual void addVertexProperty(te::dt::Property* p) = 0;

        /*!
          \brief Remove a property associated to the vertex element

          \param idx Index of the property 
        */
        virtual void removeVertexProperty(int idx) = 0;

        /*!
          \brief Get a vertex property given a index

          \param idx Index of the property 

          \return A property associated to the vertex element if the index is right and a null pointer in other case.

        */
        virtual te::dt::Property* getVertexProperty(int idx) = 0;

        /*!
          \brief Used to verify the number of properties associated to vertex elements

          \return  Integer value with the number of properties.

        */
        virtual int getVertexPropertySize() = 0;

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
        virtual void add(Edge* e) = 0;

        /*!
          \brief Update the edge element

          \param e Edge element

          \note This function turns the dirty flag of current GraphData to true and
                also the dirty flag of the edge.

         */
        virtual void update(Edge* e) = 0;

        /*!
          \brief This function removes the edge element from graph, also was removed
                 in data source.

          \param id Edge identification

         */
        virtual void removeEdge(int id) = 0;

        /*!
          \brief It returns the edge element if it's exist.

          \param id Vertex identification

          \return A valid vertex point if the element was found and a null pointer in other case.
        */
        virtual te::graph::Edge* getEdge(int id) = 0;

         /*!
          \brief Add a new property associated to the edge element

          param p  New property to be associated with edge elements.

          \note It's important before using this function call the flush() function, its necessary
                to force the memory clear and the elements will be loaded with the right size of
                properties.
        */
        virtual void addEdgeProperty(te::dt::Property* p) = 0;

        /*!
          \brief Remove a property associated to the edge element

          \param idx Index of the property 
        */
        virtual void removeEdgeProperty(int idx) = 0;

        /*!
          \brief Get a edge property given a index

          \param idx Index of the property 

          \return A property associated to the edge element if the index is right and a null pointer in other case.

        */
        virtual te::dt::Property* getEdgeProperty(int idx) = 0;

        /*!
          \brief Used to verify the number of properties associated to edge elements

          \return  Integer value with the number of properties.

        */
        virtual int getEdgePropertySize() = 0;

        //@}

        /*!
          \brief Function used to access the graph metadata

          \return A pointer to a class that defines the graph metadata

        */
        virtual te::graph::GraphMetadata* getMetadata() = 0;

        /*!
          \brief Function used to clear the memory cache, all elements was released from
                 memory, if any element was changes it will be saved.

          \return 

        */
        virtual void flush() = 0;
    };

  } // end namespace graph
} // end namespace te

#endif // __TERRALIB_GRAPH_INTERNAL_ABSTRACTGRAPH_H
