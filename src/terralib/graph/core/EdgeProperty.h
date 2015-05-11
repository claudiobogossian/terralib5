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
  \file EdgeProperty.h

  \brief This class is used to define a set of properties
         of a Edge.
*/

#ifndef __TERRALIB_GRAPH_INTERNAL_EDGEPROPERTY_H
#define __TERRALIB_GRAPH_INTERNAL_EDGEPROPERTY_H

// Terralib Include
#include "../../datatype/CompositeProperty.h"
#include "../Config.h"


// STL Includes
#include <vector>

namespace te
{
  namespace graph
  {

    /*!
      \class EdgeProperty

      \brief This class is used to define a set of properties
         of a Edge.

    */

    class TEGRAPHEXPORT EdgeProperty : public te::dt::CompositeProperty
    {
      public:

        /*! \brief Default constructor. */
        EdgeProperty();

        /*! \brief Default destructor. */
        ~EdgeProperty();
    };
  } // end namespace graph
} // end namespace te

#endif // __TERRALIB_GRAPH_INTERNAL_EDGEPROPERTY_H