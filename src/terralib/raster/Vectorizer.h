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
  \file terralib/raster/Vectorizer.h

  \brief It implements the vectorizer, based on TerraLib 4 algorithm.
*/

#ifndef __TERRALIB_RASTER_INTERNAL_VECTORIZER_H
#define __TERRALIB_RASTER_INTERNAL_VECTORIZER_H

// The 8 cardinals directions used by vectorize method
#define NORTHWEST 0
#define NORTH     1
#define NORTHEAST 2
#define EAST      3
#define SOUTHEAST 4
#define SOUTH     5
#define SOUTHWEST 6
#define WEST      7

// TerraLib
#include "../geometry/Coord2D.h"
#include "../geometry/LineString.h"
#include "../sam/rtree.h"
#include "Enums.h"
#include "Raster.h"
#include "VectorizerPolygonStructure.h"

// STL
#include <vector>

namespace te
{
  namespace rst
  {
// Forward declaration.
    class Raster;

    /*!
      \class Vectorizer

      \brief It implements the vectorizer, based on TerraLib 4 algorithm.

      This algorithm implements a vectorizer, which is an algorithm to convert an
      image with pixel-labels into a set of polygons. All connected pixels in the
      image with the same label will define an individual polygon. This algorithm
      is based on the implementation of the previous version of this library, the
      TerraLib 4.

      \ingroup rst

      \sa Raster, Geometry
    */
    class TERASTEREXPORT Vectorizer
    {
      public:

        /*!
          \brief Constructor.

          \param r      The input raster.
          \param b      The selected band of the raster to be vectorized.
          \param mp     The maximum allowed number of polygons to be created (default = 0, unlimited).
        */
        Vectorizer(Raster* r, std::size_t b, unsigned int mp = 0);

        /*!
          \brief Copy constructor.

          \param rhs The right-hand-side copy that would be used to copy from.
        */
        Vectorizer(const Vectorizer& rhs);

        /*! \brief Destructor. */
        ~Vectorizer();

        /*!
          \brief Assignment operator.

          \param rhs The right-hand-side copy that would be used to copy from.

          \return A reference to this object.
        */
        Vectorizer& operator=(const Vectorizer& rhs);
        
        /*!
          \brief Returns true if current algorithm implementation runs ok, false otherwise.

          \param polygons    The vector of polygons (will be cleared) to get the result of the vectorization.
          
          \note The caller of this method must take the ownership of the returned geometries and must delete them when necessary.
        */
        bool run(std::vector<te::gm::Geometry*>& polygons);

      protected :

        /*!
          \brief Tests if the current point is a edge start.

          \param x X coord.
          \param y Y coord.

          \return true if the current point is a edge start, false if not.
        */
        bool startingEdgeTest(const int& x, const int& y);

        /*!
          \brief Detects an edge of a cell in Raster.

          \param i abscissa (column) of the upper-left point of the shape
          \param j ordinate (line) of the upper-left point of the shape
          \param line 2D Line.

          \return true if ok, otherwise false
        */
        bool detectEdge(long i, long j, te::gm::LinearRing& line);



        /*! \brief Clear all internally allocated resources. */
        void clear();

      protected:

        double m_noDataValue;               //!< The used dummy value.
        Raster* m_rasterPtr;                   //!< A pointer to the input image.
        te::gm::Coord2D m_directions[8];    //!< Directions vector.
        double m_resX;                      //!< Resolution X.
        double m_resY;                      //!< Resolution Y.
        unsigned long m_nLines;             //!< The number of lines.
        unsigned long m_nColumns;           //!< The number of columns.
        unsigned int m_rasterBand;          //!< The raster band to be used.
        unsigned int m_maxPolygons;         //!< The maximum allowed number of polygons to be created.
        te::sam::rtree::Index<unsigned int, 8, 4>* m_rTreePolygons;   //!< A RTree instance pointer to optimize the searching of points inside already created polygons
        std::vector<VectorizerPolygonStructure> m_containerPolygons; //!< Vector of all polygons.
    };
  } // end namespace rst
}   // end namespace te

#endif  // __TERRALIB_RASTER_INTERNAL_VECTORIZER_H
