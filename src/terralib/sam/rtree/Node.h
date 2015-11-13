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
  \file Node.h

  \brief A class that represents an R-tree node.
*/

#ifndef __TERRALIB_SAM_RTREE_INTERNAL_NODE_H
#define __TERRALIB_SAM_RTREE_INTERNAL_NODE_H

// TerraLib
#include "Branch.h"

namespace te
{
  namespace sam
  {
    namespace rtree
    {
      /*!
        \class Node
        
        \brief A class that represents an R-tree node.

        Level 0 indicates that this is a leaf node, other values indicate that it is an internal node.       
      */
      template<class DATATYPE, int MAXNODES = 8, int MINNODES = MAXNODES / 2> class Node
      {
        public:

          typedef typename te::sam::rtree::Branch<Node, DATATYPE> BranchType;

          /*! \brief Constructor. */
          Node()
            : m_count(0), m_level(-1)
          {
          }

          /*!
            \brief It returns true if this is a internal node.
            
            \return True if this is a internal node and false if it is a leaf.
          */
          bool isInternalNode() const
          {
            return (m_level > 0);
          }

          /*!
            \brief It returns true if this is a leaf node.

            \return True if this is a leaf node and false if it is internal.
          */
          bool isLeaf() const
          {
            return (m_level == 0);
          }

          /*! \brief This method is used during split when a node retained and used again (beeing re-filled). */
          void init()
          {
            m_count = 0;
            m_level = -1;
            for(unsigned int i = 0; i < MAXNODES; ++i)
            {
              m_branch[i].m_child = 0;
              m_branch[i].m_mbr = te::gm::Envelope();
            }
          }

        private:

          /*!
            \brief No copy constructor allowed.

            \param rhs The other geometry.
           */
          Node(const Node& rhs);

          /*!
            \brief No assignment operator allowed.

            \param rhs The other geometry.

            \return A reference for this.
           */
          Node& operator=(const Node& rhs);


        public:

          int m_count;                    //!< The number of elements in the node (count).
          int m_level;                    //!< Leaf is zero, others positive.
          BranchType m_branch[MAXNODES];  //!< Branches.
      };

    } // end namespace rtree
  }   // end namespace sam
}     // end namespace te


#endif  // __TERRALIB_SAM_RTREE_INTERNAL_NODE_H

