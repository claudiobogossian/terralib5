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
  \file Vertex.h

  \brief From the point of view of graph theory, vertices are treated 
        as featureless and indivisible objects, although they may have 
        additional structure depending on the application from which the 
        graph arises;for instance, a semantic network is a graph in 
        which the vertices represent concepts or classes of objects.
*/

#ifndef __TERRALIB_GRAPH_INTERNAL_VERTEX_H
#define __TERRALIB_GRAPH_INTERNAL_VERTEX_H

// Terralib Includes
#include "../Config.h"
#include "../Enums.h"

// STL Includes
#include <map>
#include <string>
#include <set>
#include <vector>




namespace te
{
  // Forward declarations
  namespace dt { class AbstractData; }

  namespace graph
  {

    /*!
      \class Vertex

      \brief  From the point of view of graph theory, vertices are treated 
            as featureless and indivisible objects, although they may have 
            additional structure depending on the application from which the 
            graph arises;for instance, a semantic network is a graph in 
            which the vertices represent concepts or classes of objects.


      \note The neighbour informartion has to be defined in
              a appropriated graph implementation.
    */

    class TEGRAPHEXPORT Vertex
    {
      public:

        /*!
          \brief Constructor.

          \param id The vertex identifier

          \param isNew  Flag used to indicate that the element is new

        */
        Vertex(int id, bool isNew = true);

        /*! \brief Copy  constructor. */
        Vertex(te::graph::Vertex* rhs);

        /*! \brief Default destructor. */
        ~Vertex();

        /*!
          \brief It returns the vertex id

          \return Integer with the vertex identifier
        */
        int getId();


         /** @name Vertex Attribute Methods
         *  Method used to manager attributes from vertex object.
         */
        //@{

        /*!
          \brief It returns the vector of attributes associated with this element

          \return A vector of AbstractData (can be any type of data)
        */
        std::vector<te::dt::AbstractData*>& getAttributes();

        /*!
          \brief This function is used to set the number of attributes associated with the vertex elements

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
          \brief Returns the Predecessors vector

          \note This information only will be defined in the apropriated graph type

          \return std::vector<int> reference 

         */
        std::set<int>& getPredecessors();

        /*!
          \brief Returns the Successors vector

          \note This information only will be defined in the apropriated graph type

          \return std::vector<int> reference  

         */
        std::set<int>& getSuccessors();

        /*!
          \brief Returns the Neighborhood vector

          \note This information only will be defined in the apropriated graph type

          \return std::vector<int> reference  

         */
        std::set<int>& getNeighborhood();

        /*!
          \brief Flag used to indicate that this element was changed

          \param flag  Boolean value used to indicate the vertex state
        */
        void setDirty(bool flag);

        /*!
          \brief Used to verify the vertex state.

          \return Boolean value used to indicate the vertex state
        */
        bool isDirty();

        /*!
          \brief Flag used to indicate that this element was a new one

          \return Boolean value used to indicate the vertex state
        */
        bool isNew();

        //@}

      protected:
        int m_vertexId;                               //!< This is the vertex unique identifier.

        std::vector<te::dt::AbstractData*> m_attrs;   //!< This is the list of all vertex attributes.

        std::set<int> m_predecessors;              //!< List of all input edges (used in bidirectional graph).

        std::set<int> m_successors;                //!< List of all output edges (used in directional graph).

        std::set<int> m_neighborhood;              //!< List of all neighbors edges (used in undirectional graph).

        bool m_dirty;                                 //!< Flag used to indicate that vertex was changed.

        bool m_new;                                   //!< Flag used to indicate if this element is a new one.
    };
  } // end namespace graph
} // end namespace te

#endif // __TERRALIB_GRAPH_INTERNAL_VERTEX_H
