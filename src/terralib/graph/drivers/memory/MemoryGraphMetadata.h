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
  \file MemoryGraphMetadata.h

  \brief Class used to define the graph metadata informations
         over a memory source
*/

#ifndef __TERRALIB_GRAPH_INTERNAL_MEMORYGRAPHMETADATA_H
#define __TERRALIB_GRAPH_INTERNAL_MEMORYGRAPHMETADATA_H

// Terralib Includes
#include "../../core/GraphMetadata.h"
#include "../../Config.h"


// STL Includes
#include <string>

namespace te
{
  namespace graph
  {
    class GraphMetadata;
  
    /*!
      \class MemoryGraphMetadata

      \brief Class used to define the graph metadata informations
         over a memory source

      \sa GraphMetadata, Enums
    */

    class TEGRAPHEXPORT MemoryGraphMetadata : public GraphMetadata
    {
      public:

        /*! \brief Default constructor. */
        MemoryGraphMetadata();

        /*! \brief Default destructor. */
        ~MemoryGraphMetadata();

        /*!
          \brief Function used to load the graph information given a graph id

          \param id The Graph identifier

          \exception Exception It throws an exception if graph id equal -1
        */
        void load(int id);

        /*!
          \brief Function used to save the graph information

        */
        void save();

        /*!
          \brief Function used to update the graph information on a data source

          \exception Exception It throws an exception if graph id equal -1
        */
        void update();

    };
  } // end namespace graph
} // end namespace te

#endif // __TERRALIB_GRAPH_INTERNAL_MEMORYGRAPHMETADATA_H
