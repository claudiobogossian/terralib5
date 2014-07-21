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
  \file GeneralizedProximityMatrix.h

  \brief This class defines the GPM class.
*/

#ifndef __TERRALIB_SA_INTERNAL_GENERALIZEDPROXIMITYMATRIX_H
#define __TERRALIB_SA_INTERNAL_GENERALIZEDPROXIMITYMATRIX_H

// Terralib Includes
#include "../../graph/core/AbstractGraph.h"
#include "../Config.h"

// STL Includes
#include <memory>
#include <string>

namespace te
{
  // Forward declarations

  namespace sa
  {
  
    /*!
      \class GeneralizedProximityMatrix

      \brief This class defines a Generalized Proximity Matrix.

      \sa GPMBuilder
    */

    class TESAEXPORT GeneralizedProximityMatrix
    {
      public:

        /*! \brief Default constructor. */
        GeneralizedProximityMatrix();

        /*! \brief Virtual destructor. */
        virtual ~GeneralizedProximityMatrix();

        
        /** @name Methods
         *  Methods used by the GPM
         */
        //@{

        void setDataSetName(const std::string& dataSetName);

        std::string getDataSetName();

        void setAttributeName( const std::string& attrName);

        std::string getAttributeName();

        void setGraph(te::graph::AbstractGraph* graph);

        te::graph::AbstractGraph* getGraph();

        //@}

      private:

        std::string m_dataSetName;     //!< Attribute used to identify the dataset associated to this gpm.
        std::string m_attributeName;   //!< Attribute used to identify the attr from dataset associated to this gmp.

        std::auto_ptr<te::graph::AbstractGraph> m_graph;  //!< Graph that represents the gpm.

    };

  } // end namespace sa
} // end namespace te

#endif //__TERRALIB_SA_INTERNAL_GENERALIZEDPROXIMITYMATRIX_H
