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
  \file BoxIterator.h

  \brief This class defines a commun interface to represents a graph
         iterator class. The main diferency to anothers iterators is
         the possibility to iterate over the edges or vertexs from a 
         graph.

         The box strategy is used to select a restrict area in a graph
         and a iterator will be create accessing all elements inside
         this area.
*/

#ifndef __TERRALIB_GRAPH_INTERNAL_BOXITERATOR_H
#define __TERRALIB_GRAPH_INTERNAL_BOXITERATOR_H

// Terralib Includes
#include "../Config.h"
#include "AbstractIterator.h"

// STL Includes
#include <vector>

namespace te
{
  namespace gm 
  {
    class Envelope;
  }

  namespace graph
  {
    //forward declarations
    class Edge;
    class Vertex;
    
    /*!
      \class BoxIterator

      \brief This class defines a commun interface to represents a graph
             iterator class. The main diferency to anothers iterators is
             the possibility to iterate over the edges or vertexs from a 
             graph.

             The box strategy is used select a restrict area in a graph
             and a iterator will be create accessing all elements inside
             this area.

      \sa AbstractIterator
    */

    class TEGRAPHEXPORT BoxIterator : public AbstractIterator
    {
      public:

        /*! 
          \brief Default constructor. 

          \param g    Pointer to a valid graph implementation
          
        */
        BoxIterator(te::graph::AbstractGraph* g);

       /*! 
          \brief Constructor. 

          \param g    Pointer to a valid graph implementation

          \param e    Object used to define a restrict area

        */
        BoxIterator(te::graph::AbstractGraph* g, te::gm::Envelope* e);

        /*! \brief Virtual destructor. */
        virtual ~BoxIterator();

        
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

        te::gm::Envelope* m_extent;   //|< Attribute to define the extent clause used as query by the iterator
    };

  } // end namespace graph
} // end namespace te

#endif // __TERRALIB_GRAPH_INTERNAL_BOXITERATOR_H
