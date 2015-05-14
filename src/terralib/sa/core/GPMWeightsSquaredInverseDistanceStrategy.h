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
  \file GPMWeightsSquaredInverseDistanceStrategy.h

  \brief This class defines a class to calculates a weight for a GPM using Squared Inverse Distance strategy.
*/

#ifndef __TERRALIB_SA_INTERNAL_GPMWEIGHTSQUAREDINVERSEDISTANCESTRATEGY_H
#define __TERRALIB_SA_INTERNAL_GPMWEIGHTSQUAREDINVERSEDISTANCESTRATEGY_H

// Terralib Includes
#include "../Config.h"
#include "../Enums.h"
#include "GPMWeightsAbstractStrategy.h"

// STL Includes
#include <memory>
#include <string>

namespace te
{
  namespace sa
  {
    // Forward declarations
    class GeneralizedProximityMatrix;
  
    /*!
      \class GPMWeightsSquaredInverseDistanceStrategy

      \brief This class defines a class to calculates a weight for a GPM using Inverse Distance strategy.

      Formula: w = (a*1/(dist_centroids)2 + b*1/(dist_to_net)2 + c*1/(dist_net_connection)2)*factor

      These values can be normalized or not.

      \sa GPMBuilder
    */

    class TESAEXPORT GPMWeightsSquaredInverseDistanceStrategy : public GPMWeightsAbstractStrategy
    {
      public:

        /*! \brief Default constructor. */
        GPMWeightsSquaredInverseDistanceStrategy(bool normalize, double a = 1.0, double factor = 1.0);

        /*! \brief Virtual destructor. */
        virtual ~GPMWeightsSquaredInverseDistanceStrategy();

        
        /** @name Methods
         *  Methods used by the GPM constructor
         */
        //@{

      public:

        virtual void calculate(GeneralizedProximityMatrix* gpm);

        //@}

      protected:

        bool m_normalize;

        double m_a;

        double m_factor;
    };

  } // end namespace sa
} // end namespace te

#endif //__TERRALIB_SA_INTERNAL_GPMWEIGHTSQUAREDINVERSEDISTANCESTRATEGY_H