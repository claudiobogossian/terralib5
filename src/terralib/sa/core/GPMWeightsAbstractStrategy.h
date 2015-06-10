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
  \file GPMWeightsAbstractStrategy.h

  \brief This class defines a an Abstract class to calculates a weight for a GPM.
*/

#ifndef __TERRALIB_SA_INTERNAL_GPMWEIGHTSABSTRACTSTRATEGY_H
#define __TERRALIB_SA_INTERNAL_GPMWEIGHTSABSTRACTSTRATEGY_H

// Terralib Includes
#include "../Config.h"
#include "../Enums.h"

// STL Includes
#include <memory>
#include <string>

namespace te
{
  namespace graph 
  { 
    class Edge; 
    class GraphMetadata;
  }

  namespace sa
  {
    // Forward declarations
    class GeneralizedProximityMatrix;
  
    /*!
      \class GPMWeightsAbstractStrategy

      \brief This class defines a an Abstract class to calculates a weight for a GPM.

      \sa GPMBuilder
    */

    class TESAEXPORT GPMWeightsAbstractStrategy
    {
      public:

        /*! \brief Default constructor. */
        GPMWeightsAbstractStrategy();

        /*! \brief Virtual destructor. */
        virtual ~GPMWeightsAbstractStrategy();

        
        /** @name Methods
         *  Methods used by the GPM constructor
         */
        //@{

      public:

        GPMWeightsStrategyType getWeightsType();

        virtual void calculate(GeneralizedProximityMatrix* gpm) = 0;

      protected:

        /*! \brief Added to the edge a new attribute for weight information and return the attr index*/
        int createWeightAttribute(GeneralizedProximityMatrix* gpm);

        /*! \brief Function used to get the distance attribute index. */
        bool getDistanceAttributeIndex(te::graph::GraphMetadata* gm, int& index);

        /*! \brief Function used to get the distance value from a edge element. */
        double getDistanceAttributeValue(te::graph::Edge* e, const int index);

        //@}

      protected:

        GPMWeightsStrategyType m_type;       //!< Weight Type.

        GeneralizedProximityMatrix* m_gpm;   //!< GPM Pointer.
    };

  } // end namespace sa
} // end namespace te

#endif //__TERRALIB_SA_INTERNAL_GPMWEIGHTSABSTRACTSTRATEGY_H