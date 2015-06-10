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
  \file GPMConstructorAbstractStrategy.h

  \brief This class defines a an Abstract class for a GPM constructor.
*/

#ifndef __TERRALIB_SA_INTERNAL_GPMCONSTRUCTORABSTRACTSTRATEGY_H
#define __TERRALIB_SA_INTERNAL_GPMCONSTRUCTORABSTRACTSTRATEGY_H

// Terralib Includes
#include "../Config.h"
#include "../Enums.h"

// STL Includes
#include <memory>
#include <string>

namespace te
{
  // Forward declarations
  namespace da 
  {
    class DataSource; 
    class DataSet;
  }

  namespace sa
  {
    // Forward declarations
    class GeneralizedProximityMatrix;
  
    /*!
      \class GPMConstructorAbstractStrategy

      \brief This class defines a an Abstract class for a GPM constructor.

      \sa GPMBuilder
    */

    class TESAEXPORT GPMConstructorAbstractStrategy
    {
      public:

        /*! \brief Default constructor. */
        GPMConstructorAbstractStrategy();

        /*! \brief Virtual destructor. */
        virtual ~GPMConstructorAbstractStrategy();

        
        /** @name Methods
         *  Methods used by the GPM constructor
         */
        //@{

      public:

        GPMConstructorStrategyType getConstructorType();

        void construct(te::da::DataSource* ds, GeneralizedProximityMatrix* gpm);

      protected:

        /*! \brief Function used to create all vertex object based on data set */
        void createVertexObjects();

        /*! \brief Added to the edge a new attribute for distance information */
        void createDistanceAttribute(GeneralizedProximityMatrix* gpm);

        /*! \brief Function used to generated the edge id */
        int getEdgeId();

        /*! \brief Build the edges using specific strategy. */
        virtual void constructStrategy() = 0;

        //@}

      protected:

        GPMConstructorStrategyType m_type;   //!< Constructor Type.

        int m_edgeId;                        //!< Attribute used as a index counter for edge objects

        te::da::DataSource* m_ds;            //!< Data Source pointer.

        GeneralizedProximityMatrix* m_gpm;   //!< GPM Pointer.
    };

  } // end namespace sa
} // end namespace te

#endif //__TERRALIB_SA_INTERNAL_GPMCONSTRUCTORABSTRACTSTRATEGY_H