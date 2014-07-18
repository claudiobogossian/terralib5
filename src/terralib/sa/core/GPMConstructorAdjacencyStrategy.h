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
  \file GPMConstructorAdjacencyStrategy.h

  \brief This class defines a an adjacency strategy class for a GPM constructor.
*/

#ifndef __TERRALIB_SA_INTERNAL_GPMCONSTRUCTORADJACENCYSTRATEGY_H
#define __TERRALIB_SA_INTERNAL_GPMCONSTRUCTORADJACENCYSTRATEGY_H

// Terralib Includes
#include "../Config.h"
#include "GPMConstructorAbstractStrategy.h"

// STL Includes
#include <memory>

namespace te
{
  namespace sa
  {
  
    /*!
      \class GPMConstructorAdjacencyStrategy

      \brief This class defines a an adjacency strategy class for a GPM constructor.

      \sa GPMBuilder
    */

    class TESAEXPORT GPMConstructorAdjacencyStrategy : public te::sa::GPMConstructorAbstractStrategy
    {
      public:

        /*! \brief Default constructor. */
        GPMConstructorAdjacencyStrategy();

        /*! \brief Specific constructor. */
        GPMConstructorAdjacencyStrategy(bool calcDistance);

        /*! \brief Virtual destructor. */
        virtual ~GPMConstructorAdjacencyStrategy();

        
        /** @name Methods
         *  Methods used by the GPM constructor
         */
        //@{

        virtual void constructStrategy();

        //@}

      protected:

        bool m_calcDistance;
    };

  } // end namespace sa
} // end namespace te

#endif //__TERRALIB_SA_INTERNAL_GPMCONSTRUCTORADJACENCYSTRATEGY_H