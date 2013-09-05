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
  \file BoxLoaderStrategy.h

  \brief This class implements the main functions necessary to
        save and load the graph data and metadata information
        using as strategy a bounding box to create a region
        that defines a group of elements.
*/

#ifndef __TERRALIB_GRAPH_INTERNAL_BOXLOADERSTRATEGY_H
#define __TERRALIB_GRAPH_INTERNAL_BOXLOADERSTRATEGY_H

// Terralib Includes
#include "../Config.h"
#include "Enums.h"

#include "AbstractGraphLoaderStrategy.h"

#include <string>

namespace te
{
  // Forward declarations
  namespace da 
  { 
    class DataSet;
  }

  namespace gm 
  { 
    class Envelope;
    class GeometryProperty;
    class Point;
  }

  namespace graph
  {
    //forward declarations
    class AbstractGraph;
    class GraphCache;
    class GraphData;
    class GraphMetadata;

    /*!
      \class BoxLoaderStrategy

      \brief This class implements the main functions necessary to
            save and load the graph data and metadata information
            using as strategy a bounding box to create a region
            that defines a group of elements.

      \sa AbstractGraphLoaderStrategy
    */

    class TEGRAPHEXPORT BoxLoaderStrategy : public AbstractGraphLoaderStrategy
    {
      public:

        /*! \brief Default constructor. */
        BoxLoaderStrategy(te::graph::GraphMetadata* metadata);

        /*! \brief Default destructor. */
        virtual ~BoxLoaderStrategy();

         /** @name Database Box Graph Loader Strategy Methods
        *  Method used to manager a graph data
        */
        //@{

        /*!
        \brief Functio used to load a group of vertex elements given a base element

        \param vertexId   Attribute used to identify the base element

        \param g          Pointer to a graph, parent of this element

        \param gc         This is a optional attribute, if present the cache will be check if the already been loaded

        */
        virtual void loadDataByVertexId(int vertexId, te::graph::AbstractGraph* g, te::graph::GraphCache* gc = 0);

        /*!
        \brief Functio used to load a group of edges elements given a base element

        \param edgeId     Attribute used to identify the base element

        \param g          Pointer to a graph, parent of this element

        \param gc         This is a optional attribute, if present the cache will be check if the already been loaded

        */
        virtual void loadDataByEdgeId(int edgeId, te::graph::AbstractGraph* g, te::graph::GraphCache* gc = 0);

        //@}

      protected:

        /*!
          \brief Generate a box with p as center. The new box will have n% of the total graph height and n% of total graph width

          \param p Terralib object that defines a geometric point.

          \param tableName  Attribute used to define the table name that contains the graph elements

          \return Terralib object that defines geometric region.
        */
        te::gm::Envelope* calculateBox(te::gm::Point* p, std::string tableName);
    };
  } // end namespace graph
} // end namespace te

#endif // __TERRALIB_GRAPH_INTERNAL_BOXLOADERSTRATEGY_H