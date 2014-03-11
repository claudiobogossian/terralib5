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
  \file Memoryterator.h

  \brief This class defines a commun interface to represents a graph
         iterator class. The main diferency to anothers iterators is
         the possibility to iterate over the edges or vertexs from a 
         graph.
*/

#ifndef __TERRALIB_GRAPH_INTERNAL_MEMORYITERATOR_H
#define __TERRALIB_GRAPH_INTERNAL_MEMORYITERATOR_H

// Terralib Includes
#include "../Config.h"
#include "AbstractIterator.h"


// STL Includes
#include <map>

namespace te
{
  namespace graph
  {
    //forward declarations
    class AbstractGraph;
    class Edge;
    class Vertex;
    
    /*!
      \class Memoryterator

      \brief This class defines a commun interface to represents a graph
         iterator class. The main diferency to anothers iterators is
         the possibility to iterate over the edges or vertexs from a 
         graph.

      \sa 
    */

    class TEGRAPHEXPORT MemoryIterator : public AbstractIterator
    {
      public:

        /*! \brief Default constructor. */
        MemoryIterator(te::graph::AbstractGraph* g);

        /*! \brief Virtual destructor. */
        virtual ~MemoryIterator();

        
        /** @name Vertex Iterator Methods
         *  Method used to access vertex elements from a graph.
         */
        //@{

        /*!
          \brief It returns a pointer to the first vertex element of a graph

          \return A valid vertex point if the element was found and a null pointer in other case.
        */
        virtual te::graph::Vertex* getFirstVertex();

        /*!
          \brief It returns a pointer to the next vertex element of a graph

          \return A valid vertex point if the element was found and a null pointer in other case.
        */
        virtual te::graph::Vertex* getNextVertex();

        /*!
          \brief It returns a pointer to the previous vertex element of a graph

          \return A valid vertex point if the element was found and a null pointer in other case.
        */
        virtual te::graph::Vertex* getPreviousVertex();

        /*!
          \brief Used to check the iterator position

          \return True if the iterator is at after the end and false in other case
        */
        virtual bool isVertexIteratorAfterEnd();

        /*!
          \brief It returns the number of elements of this iterator

          \return Size t value.
        */
        virtual size_t getVertexInteratorCount();

        //@}

        /** @name Vertex Iterator Methods
         *  Method used to access vertex elements from a graph.
         */
        //@{

        /*!
          \brief It returns a pointer to the first edge element of a graph

          \return A valid edge point if the element was found and a null pointer in other case.
        */
        virtual te::graph::Edge* getFirstEdge();

        /*!
          \brief It returns a pointer to the next edge element of a graph

          \return A valid edge point if the element was found and a null pointer in other case.
        */
        virtual te::graph::Edge* getNextEdge();

        /*!
          \brief It returns a pointer to the previous edge element of a graph

          \return A valid edge point if the element was found and a null pointer in other case.
        */
        virtual te::graph::Edge* getPreviousEdge();

        /*!
          \brief Used to check the iterator position

          \return True if the iterator is at after the end and false in other case
        */
        virtual bool isEdgeIteratorAfterEnd();

        /*!
          \brief It returns the number of elements of this iterator

          \return Size t value.
        */
        virtual size_t getEdgeInteratorCount();

        //@}

      protected:

        std::map<int, Vertex*>       m_vertexMap;      //!< This map contains all vertexs from this graph.
        std::map<int, Edge*>         m_edgeMap;        //!< This map contains all edges from this graph.

        std::map<int, Vertex*>::iterator  m_vertexMapIt;      //!< Iterator for all vertexs from this graph.
        std::map<int, Edge*>::iterator    m_edgeMapIt;        //!< Iterator for all edges from this graph.

    };

  } // end namespace graph
} // end namespace te

#endif // __TERRALIB_GRAPH_INTERNAL_MEMORYITERATOR_H
