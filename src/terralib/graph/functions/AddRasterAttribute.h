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
  \file AddRasterAttribute.h

  \brief This class defines a function used to add to a graph the raster pixel value as attribute

*/

#ifndef __TERRALIB_GRAPH_INTERNAL_ADDRASTERATTRIBUTE_H
#define __TERRALIB_GRAPH_INTERNAL_ADDRASTERATTRIBUTE_H

// Terralib Includes
#include "../Config.h"


// STL Includes
#include <string>
#include <set>

namespace te
{

  namespace rst { class Raster; }

  namespace graph
  {
    // Forward declaration
    class AbstractGraph;
    class Vertex;

    /*!
      \class AddRasterAttribute

      \brief This class defines a function used to add to a graph the raster pixel value as attribute

    */

    class TEGRAPHEXPORT AddRasterAttribute
    {
      public:

        /*! 
          \brief Default constructor. 

          \param graph    Pointer to a bidirectional graph

          \param  attributeName The name used to represent the attribute 
        
        */
        AddRasterAttribute(te::graph::AbstractGraph* graph, std::string spatialAttributeName, std::string attributeName, te::rst::Raster* raster, int band);

        /*! \brief Virtual destructor. */
        virtual ~AddRasterAttribute();

    };

  } // end namespace graph
} // end namespace te

#endif // __TERRALIB_GRAPH_INTERNAL_ADDRASTERATTRIBUTE_H