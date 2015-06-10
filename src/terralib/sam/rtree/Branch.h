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
  \file Branch.h

  \brief A struct that represents a node-branch in an R-tree.
*/

#ifndef __TERRALIB_SAM_RTREE_INTERNAL_BRANCH_H
#define __TERRALIB_SAM_RTREE_INTERNAL_BRANCH_H

// TerraLib
#include "../../geometry/Envelope.h"

namespace te
{
  namespace sam
  {
    namespace rtree
    {
      /*!
        \struct Branch
        
        \brief It represents a node-branch of a Rtree.

        \warning The types that can be used in DATATYPE template are very strict: int, unsigned, long and pointers.
      */
      template<class NODE, class DATATYPE> struct Branch
      {
        te::gm::Envelope m_mbr;  //!< Bounding box containing all the objects under the branch or an object bounding box.

        union
        {
          NODE* m_child;    //!< A pointer to the child node.
          DATATYPE m_data;  //!< An object-identifier (oid).
        };
      };

    } // end namespace rtree
  }   // end namespace sam
}     // end namespace te


#endif  // __TERRALIB_SAM_RTREE_INTERNAL_BRANCH_H

