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
  \file QueryIterator.h

  \brief This class defines a commun interface to represents a graph
         iterator class. The main diferency to anothers iterators is
         the possibility to iterate over the edges or vertexs from a 
         graph.

         The query strategy is used to select a restrict group of
         elements given a "where clause".
*/

#ifndef __TERRALIB_GRAPH_INTERNAL_QUERYITERATOR_H
#define __TERRALIB_GRAPH_INTERNAL_QUERYITERATOR_H

// Terralib Includes
#include "../Config.h"
#include "AbstractIterator.h"

// STL Includes
#include <vector>

namespace te
{
  namespace da { class Expression; }

  namespace graph
  {
    //forward declarations
    class Edge;
    class Vertex;
    
    /*!
      \class QueryIterator

      \brief This class defines a commun interface to represents a graph
         iterator class. The main diferency to anothers iterators is
         the possibility to iterate over the edges or vertexs from a 
         graph.

         The query strategy is used to select a restrict group of
         elements given a "where clause".

      \sa AbstractIterator
    */

    class TEGRAPHEXPORT QueryIterator : public AbstractIterator
    {
      public:

        /*! 
          \brief Default constructor. 

          \param g    Pointer to a valid graph implementation
          
        */
        QueryIterator(te::graph::AbstractGraph* g);

       /*! 
          \brief Constructor.

          \param g    Pointer to a valid graph implementation

          \param e    Object used to define the where clause

        */
        QueryIterator(te::graph::AbstractGraph* g, te::da::Expression* e);

        /*! \brief Virtual destructor. */
        virtual ~QueryIterator();

        
        /** @name Vertex Access Methods
         *  Method used to access vertex elements from a graph.
         */
        //@{

        /*!
          \brief It returns a pointer to the first vertex element of a graph

          \return A valid vertex point if the element was found and a null pointer in other case.
        */
        virtual te::graph::Vertex* getFirstVertex();

        /*!
          \brief It returns a pointer to the first edge element of a graph

          \return A valid edge point if the element was found and a null pointer in other case.
        */
        virtual te::graph::Edge* getFirstEdge();

      protected:

        te::da::Expression* m_exp;   //|< Attribute to define the where clause used as query by the iterator
    };

  } // end namespace graph
} // end namespace te

#endif // __TERRALIB_GRAPH_INTERNAL_QUERYITERATOR_H
