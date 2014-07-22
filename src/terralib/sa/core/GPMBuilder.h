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
  \file GPMBuilder.h

  \brief This class defines the GPM Builder class.
*/

#ifndef __TERRALIB_SA_INTERNAL_GPMBUILDER_H
#define __TERRALIB_SA_INTERNAL_GPMBUILDER_H

// Terralib Includes
#include "../Config.h"
#include "GeneralizedProximityMatrix.h"

// STL Includes
#include <map>

namespace te
{
  // Forward declarations
  namespace da { class DataSource; }

  namespace sa
  {
    // Forward declarations
    class GPMConstructorAbstractStrategy;
    class GPMWeightsAbstractStrategy;
  
    /*!
      \class GPMBuilder

      \brief This class defines the GPM Builder class.

    */

    class TESAEXPORT GPMBuilder
    {
      public:

        /*! \brief Default constructor. */
        GPMBuilder(GPMConstructorAbstractStrategy* constructor, GPMWeightsAbstractStrategy* weights);

        /*! \brief Virtual destructor. */
        virtual ~GPMBuilder();

        
        /** @name Methods
         *  Methods used by the GPM builder
         */
        //@{

        /*!
          \brief Function used to create a empty gpm (using a MEMORY DIRECT graph)
          
          \param ds                 Data source pointer
          \param dataSetName        Container with data source information
          \param attributeName      Attribute used to define the output graph type

          \return True if the gmp was correctly generated and false in other case.

        */
        bool setGPMInfo(te::da::DataSourcePtr ds, const std::string& dataSetName, const std::string& attributeName);

        std::auto_ptr<GeneralizedProximityMatrix> build();

        //@}

      private:

        GPMConstructorAbstractStrategy* m_constructor;    //!< Strategy used to construct the gpm.

        GPMWeightsAbstractStrategy* m_weights;            //!< Strategy used to calculate the weight of a gpm.

        std::auto_ptr<GeneralizedProximityMatrix> m_gpm;  //!< GPM reference pointer.

        te::da::DataSourcePtr m_ds;                       //!< Input Data Source.

    };

  } // end namespace sa
} // end namespace te

#endif //__TERRALIB_SA_INTERNAL_GPMBUILDER_H