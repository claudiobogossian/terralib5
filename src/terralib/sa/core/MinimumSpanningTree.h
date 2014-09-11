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
  \file terralib/sa/core/MinimumSpanningTree.h

  \brief This file contains a class that represents the Minimum Spanning Tree operation.

  \reference Boost BGL.
*/

#ifndef __TERRALIB_SA_INTERNAL_MINIMUMSPANNINGTREE_H
#define __TERRALIB_SA_INTERNAL_MINIMUMSPANNINGTREE_H

// Terralib Includes
#include "../Config.h"

// STL Includes
#include <memory>

namespace te
{
  //forward declarations
  namespace graph { class AbstractGraph; }

  namespace sa
  {
    /*!
      \class SkaterOperation

      \brief Class used to execute the skater operations

    */
    class TESAEXPORT MinimumSpanningTree
    {
      public:

        /*! \brief Default constructor. */
        MinimumSpanningTree(te::graph::AbstractGraph* inputGraph);

        /*! \brief Virtual destructor. */
        ~MinimumSpanningTree();

      public:

        /*! 
          \brief Function to execute the kruskal operation.

          \param weightAttrIdx The edge weight attribute index.
          
          \return Pointer to AbstractGraph that represents the Minimum Spanning Tree from input graph.
        */
        te::graph::AbstractGraph* kruskal(int weightAttrIdx);

      protected:

        /*! \brief Function to create a empty graph with vertex attributes from gpm graph. */
        te::graph::AbstractGraph* createGraph();

      protected:

        te::graph::AbstractGraph* m_inputGraph;   //!< Pointer to input graph.

    };
  } // end namespace sa
} // end namespace te

#endif //__TERRALIB_SA_INTERNAL_MINIMUMSPANNINGTREE_H
