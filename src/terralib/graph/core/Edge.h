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
  \file Edge.h

  \brief  Class used to define the edge struct of a graph. Its compose
          with a identifier, the vertex origin and destiny information,
          a set of properties if exist and flags to inform his state
          (new or dirty).
*/

#ifndef __TERRALIB_GRAPH_INTERNAL_EDGE_H
#define __TERRALIB_GRAPH_INTERNAL_EDGE_H

// Terralib Includes
#include "../Config.h"

// STL Includes
#include <string>
#include <vector>

namespace te
{
  // Forward declarations
  namespace dt { class AbstractData; }

  namespace graph
  {
    
    /*!
      \class Edge

      \brief Class used to define the edge struct of a graph. Its compose
            with a identifier, the vertex origin and destiny information,
            a set of properties if exist and flags to inform his state
            (new or dirty).

      \sa AbstractGraph
    */

    class TEGRAPHEXPORT Edge
    {
      public:

        /*!
          \brief Constructor.

          \param id Edge identifier

          \param vFrom The vertex origin Id

          \param vTo The vertex destiny Id

          \param isNew  Flag used to indicate that the element is new

        */
        Edge(int id, int vFrom, int vTo, bool isNew = true);

        /*! \brief Copy  constructor. */
        Edge(te::graph::Edge* rhs);

        /*! \brief Default destructor. */
        ~Edge();

        /*!
          \brief It returns the edge identification

          \return Integer value with the edge Id
        */
        int getId();

        /*!
          \brief It returns the vertex origin identification 

          \return Integer value with the vertex Id
        */
        int getIdFrom();

        /*!
          \brief It returns the vertex destiny identification 

          \return Integer value with the vertex Id
        */
        int getIdTo();

        /*!
          \brief It returns the vector of attributes associated with this element

          \return A vector of AbstractData (can be any type of data)
        */
        std::vector<te::dt::AbstractData*>& getAttributes();

        /*!
          \brief This function is used to set the number of attributes associated with the edge elements

          \param size Integer value to define the attribute size

          \note Its important to use this method after a new property was associated to a edge
                Function defined in AbstractGraph addEdgeProperty.

        */
        void setAttributeVecSize(int size);

        /*!
          \brief Add a new attribute to this element

          \param idx Index of the new attribute (must be a valid position)

          \param ad AbstractData (can be any type of data)

        */
        void addAttribute(int idx, te::dt::AbstractData* ad);

        /*!
          \brief Remove a attribute associated with this element

          \param idx  Index of the attribute (must be a valid position)

        */
        void removeAttribute(int idx);

        /*!
          \brief Flag used to indicate that this element was changed

          \param flag  Boolean value used to indicate the edge state
        */
        void setDirty(bool flag);

        /*!
          \brief Used to verify the edge state.

          \return Boolean value used to indicate the edge state
        */
        bool isDirty();

        /*!
          \brief Flag used to indicate that this element was a new one

          \return Boolean value used to indicate the edge state
        */
        bool isNew();


      protected:
        int m_edgeId;                                   //!< This is the edge unique identifier.

        int m_vertexIdFrom;                             //!< Origin vertex identifier from this edge.

        int m_vertexIdTo;                               //!< Destiny vertex identifier from this edge.

        std::vector<te::dt::AbstractData*> m_attrs;     //!< This is the list of all edge attributes.

        bool m_dirty;                                   //!< Flag used to indicate that edge was changed

        bool m_new;                                     //!< Flag used to indicate if this element is a new one.
    };
  } // end namespace graph
} // end namespace te

#endif // __TERRALIB_GRAPH_INTERNAL_EDGE_H