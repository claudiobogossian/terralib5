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

          \param p      Polygon reference.
          \param v      Parameter V.
          \param tidy   Tile indexer dy.
        */
        VectorizerPolygonStructure(const te::gm::Polygon& p, const int& v, const double& tidy);

        /*! \brief Default destructor. */
        ~VectorizerPolygonStructure();

        /*! \brief Init internal variables. */
        void init();

        /*! \brief Init internal variables. */
        void clear();

        /*!
          \brief Reset the current instance.

          \param p      Polygon reference.
          \param v      Parameter V.
          \param tidy   Tile indexer dy.
        */
        void reset(const te::gm::Polygon& p, const int& v, const double& tidy);

        /*!
          \brief Assignment operator.

          \param rhs The right-hand-side copy that would be used to copy from.

          \return A reference to this object.
        */
        VectorizerPolygonStructure& operator=(const VectorizerPolygonStructure& rhs);

      public:
        int m_value;                   //!< The value (color) related to a polygon.
        double m_tileIndexerDY;        //!< The polygon tile indexer Y Axis resolution.
        TileIndexer* m_indexer;        //!< The polygon tile indexer pointer.
        te::gm::Polygon m_polygon;     //!< The stored polygon instance.

    };

  } // end namespace rst
}   // end namespace te

#endif  // __TERRALIB_RASTER_INTERNAL_VECTORIZERPOLYGONSTRUCTURE_H
