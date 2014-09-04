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
  \file terralib/sa/core/SkaterOperation.h

  \brief This file contains a class that represents the skater operation.

  \reference Adapted from TerraLib4.
*/

#ifndef __TERRALIB_SA_INTERNAL_SKATEROPERATION_H
#define __TERRALIB_SA_INTERNAL_SKATEROPERATION_H

// Terralib Includes
#include "SkaterParams.h"
#include "../Config.h"

// STL Includes
#include <memory>

namespace te
{
  //forward declarations
  namespace graph 
  { 
    class AbstractGraph;
    class Vertex; 
  }

  namespace sa
  {
    //forward declarations
    class SkaterRoot;

    /*!
      \class SkaterOperation

      \brief Class used to execute the skater operations

    */
    class TESAEXPORT SkaterOperation
    {
      public:

        /*! \brief Default constructor. */
        SkaterOperation();

        /*! \brief Virtual destructor. */
        ~SkaterOperation();

      public:

        /*! \brief Function to execute the skater operation. */
        void execute();

        void setParameters(te::sa::SkaterInputParams* inParams, te::sa::SkaterOutputParams* outParams);

      protected:

        /*! \brief Function to create the weight attribute. */
        void createWeightAttribute(int weightAttrIdx, std::vector<int> attrsIdx);

        /*! \brief Function to calculate the weight attribute using the euclidean distance. */
        double calculateWeight(std::vector<int> attrsIdx, te::graph::Vertex* vFrom, te::graph::Vertex* vTo);

        /*! \brief Function to calculate the msf of a gpm, using boost kruskal algorithm. */
        te::graph::AbstractGraph* minimumSpanningTree(int weightAttrIdx);

        /*! \brief Function to create a empty graph with vertex attributes from gpm graph. */
        te::graph::AbstractGraph* createGraph();

      protected:

        std::auto_ptr<te::sa::SkaterInputParams> m_inputParams;       //!< Attribute with the skater input parameters.

        std::auto_ptr<te::sa::SkaterOutputParams> m_outputParams;     //!< Attribute with the skater output parameters.
    };
  } // end namespace sa
} // end namespace te

#endif //__TERRALIB_SA_INTERNAL_SKATEROPERATION_H