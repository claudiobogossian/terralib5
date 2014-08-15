/*  Copyright (C) 2008-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/raster/VectorizerPolygonStructure.h

  \brief A polygon container node class.
*/

#ifndef __TERRALIB_RASTER_INTERNAL_VECTORIZERPOLYGONSTRUCTURE_H
#define __TERRALIB_RASTER_INTERNAL_VECTORIZERPOLYGONSTRUCTURE_H

// TerraLib
#include "../geometry/Polygon.h"
#include "TileIndexer.h"
#include "Config.h"

#include <memory>

namespace te
{
  namespace rst
  {
    /*!
      \class VectorizerPolygonStructure

      \brief A polygon container node class.
    */
    class TERASTEREXPORT VectorizerPolygonStructure
    {
      public:
        /*! \brief Default constructor. */
        VectorizerPolygonStructure();

        /*!
          \brief Copy constructor.

          \param rhs The right-hand side VectorizerPolygonStructure.
        */
        VectorizerPolygonStructure(const VectorizerPolygonStructure& rhs);

        /*!
          \brief Constructor with non default parameters.

          \param polPtr      Polygon pointer (this structure will take the ownership of the given pointer)
          \param v      The pixel value related to the polygon.
          \param tidy   Tile indexer dy.
        */
        VectorizerPolygonStructure( te::gm::Polygon* polPtr, const unsigned int& v, const double& tidy);

        /*! \brief Default destructor. */
        ~VectorizerPolygonStructure();

        /*! \brief Init internal variables. */
        void init();

        /*! \brief Init internal variables. */
        void clear();

        /*!
          \brief Reset the current instance.

          \param polPtr      Polygon pointer (this structure will take the ownership of the given pointer)
          \param v      Parameter V.
          \param tidy   Tile indexer dy.
          
        */
        void reset( te::gm::Polygon* polPtr, const unsigned int& v, const double& tidy);

        /*!
          \brief Assignment operator.

          \param rhs The right-hand-side copy that would be used to copy from.

          \return A reference to this object.
        */
        VectorizerPolygonStructure& operator=(const VectorizerPolygonStructure& rhs);

      public:
        
        unsigned int m_value;                   //!< The value (color) related to a polygon.
        double m_tileIndexerDY;        //!< The polygon tile indexer Y Axis resolution.
        std::auto_ptr< TileIndexer > m_indexerPtr;        //!< A pointer to the polygon tile indexer pointer.
        std::auto_ptr< te::gm::Polygon > m_polygonPtr;     //!< A pointer to the stored polygon instance.

    };

  } // end namespace rst
}   // end namespace te

#endif  // __TERRALIB_RASTER_INTERNAL_VECTORIZERPOLYGONSTRUCTURE_H
