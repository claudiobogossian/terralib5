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

  \brief Auxiliary structure when searching for a split partition.
*/

#ifndef __TERRALIB_SAM_RTREE_INTERNAL_PARTITIONVARS_H
#define __TERRALIB_SAM_RTREE_INTERNAL_PARTITIONVARS_H

// TerraLib
#include "../../geometry/Envelope.h"

namespace te
{
  namespace sam
  {
    namespace rtree
    {
      /*!
        \struct PartitionVars
        
        \brief Auxiliary structure when searching for a split partition.
      */
      template<class BRANCH, int MAXNODES> struct PartitionVars
      {
        int m_partition[MAXNODES + 1];      //!< Auxiliary partition vector.
        int m_taken[MAXNODES + 1];          //!< Flag to indicate that entry is ok.
        int m_count[2];                     //!< Number of entries in each new page.
        te::gm::Envelope m_cover[2];        //!< Auxiliary box of each new page.
        double m_area[2];                   //!< Auxiliary area of each new page.
        BRANCH m_branchBuf[MAXNODES + 1];   //!< Auxiliary branch buffer.
        te::gm::Envelope m_coverSplit;      //!< Auxiliary box covering branchBuf.

        /*! \brief Initializes partition vars. */
        void init()
        {
          m_count[0] = 0;
          m_count[1] = 0;

          for(int i = 0; i <= MAXNODES; ++i)
          {
            m_taken[i] = 0;
            m_partition[i] = -1;
          }
        }
      };

    } // end namespace rtree
  }   // end namespace sam
}     // end namespace te


#endif  // __TERRALIB_SAM_RTREE_INTERNAL_PARTITIONVARS_H

