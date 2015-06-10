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
  \file SequenceIterator.h

  \brief This class defines a commun interface to represents a graph
         iterator class. The main diferency to anothers iterators is
         the possibility to iterate over the edges or vertexs from a 
         graph.

         The sequency strategy is used to select a group of elements
         orderly.
*/

#ifndef __TERRALIB_GRAPH_INTERNAL_SEQUENCEITERATOR_H
#define __TERRALIB_GRAPH_INTERNAL_SEQUENCEITERATOR_H

// Terralib Includes
#include "../core/AbstractGraph.h"
#include "../iterator/AbstractIterator.h"
#include "../Config.h"

// STL Includes
#include <vector>

namespace te
{
  namespace graph
  {
    //forward declarations
    class Edge;
    class Vertex;
    
    /*!
      \class AbstractIterator

      \brief This class defines a commun interface to represents a graph
         iterator class. The main diferency to anothers iterators is
         the possibility to iterate over the edges or vertexs from a 
         graph.

         The sequency strategy is used to select a group of elements
         orderly.

      \sa AbstractIterator
    */

    class TEGRAPHEXPORT SequenceIterator : public AbstractIterator
    {
      public:

        /*! \brief Default constructor. */
        SequenceIterator(te::graph::AbstractGraph* g);

        /*! \brief Virtual destructor. */
        virtual ~SequenceIterator();

        
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
    };

  } // end namespace graph
} // end namespace te

#endif // __TERRALIB_GRAPH_INTERNAL_SEQUENCEITERATOR_H
