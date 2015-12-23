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
  \file terralib/raster/TileIndexer.h

  \brief Polygon tile indexing class for optmized geometrical relational tests.
*/

#ifndef __TERRALIB_RASTER_INTERNAL_TILEINDEXER_H
#define __TERRALIB_RASTER_INTERNAL_TILEINDEXER_H

// TerraLib
#include "../geometry/Point.h"
#include "../geometry/Polygon.h"
#include "../geometry/LinearRing.h"
#include "../geometry/Coord2D.h"
#include "Config.h"

// STL
#include <math.h>
#include <vector>

namespace te
{
  namespace rst
  {
    /*!
      \class TileIndexer

      \brief Polygon tile indexing class for optmized geometrical relational tests.
      
      \ingroup rst

      \note The related polygon instance must always be valid (only the polygon
            reference is stored internally).
    */
    class TERASTEREXPORT TileIndexer
    {
      public:
        /*! \typedef Indexed elements node type (pair<ring index, seg index>). */
        typedef std::vector<std::pair<unsigned int, unsigned int> > TileSegIndex;

      protected:
        /*! \brief Constructor. */
        TileIndexer(const TileIndexer&);

        /*! \brief Copy overload. */
        const TileIndexer& operator=(const TileIndexer&);

        /*!
          \brief Gets tile index intervals in y direction for a given segment.

          \param p1          First segment coordinate.
          \param p2          Second segment coordinate.
          \param firstTile   The first tile index that this segment intersects.
          \param lastTile    The last tile index that this segment intersects.
          \return true if ok, false on errors.

          \note  The segment does NOT need to be oriented.
        */
        bool getTileIndex(const te::gm::Point& p1, const te::gm::Point& p2,
                          unsigned int& firstTile, unsigned int& lastTile) const;

        /*!
          \brief Gets tile index for y coordinate value.

          \param y           Value of "y" coordinate.
          \param tileIndex   Index of corresponding tile.
          \return true if ok, false on errors.
        */
        bool getTileIndex(const double& y, unsigned int& tileIndex) const;

        /*! \brief Init internal variables. */
        void init();

      public:

        /*!
          \brief Alternative Constructor.

          \param pol    The polygon to index.
          \param dy     Tile size along "y" axis.
        */
        TileIndexer(const te::gm::Polygon& pol, const double& dy);

        /*! \brief Clear all internal resources. */
        void clear();

        /*! \brief Destructor. */
        ~TileIndexer();

        /*!
          \brief Update the tile index with the information of the supplied ring.
          
          \param ri The ring index.
          \return true, if ok, false on errors.
        */
        bool addRing(const unsigned int& ri);

        /*!
          \brief Gets tile index.

          \param y      The Y value.
          \param index  Output tile pointer.
          \return true if ok, false on errors.
        */
        bool getTile(const double& y, TileSegIndex** index) const;

        /*! \brief Returns the polygon. */
        inline const te::gm::Polygon& getPolygon() const
        {
          return m_referencePolygon;
        };
        
        /*!
          \brief It returns true if the given geometry is within the indexed reference polygon.

          \param rhs The other geometry to be compared.

          \return true if the given geometry is within the indexed reference polygon.
        */
        bool within_or_touches(const te::gm::Point& geometry) const;

      protected:
        double m_dy;                                  //!< Tile resolution along "y" axis.
        const te::gm::Polygon& m_referencePolygon;    //!< Reference polygon.
        std::vector<TileSegIndex*> m_tileIndex;       //!< Each tile segments index vector.
        
        // Variables used by the method within
        
        mutable TileSegIndex* m_withinTileIndexPtr;
        mutable double m_withinTileY; 
        mutable double m_withinTileX;
        mutable bool m_withinIsInside;
        mutable int m_withinYFlag0;
        mutable int m_withinYFlag1;
        mutable int m_withinYEquals;
        mutable te::gm::LinearRing const* m_withinRingPtr;
        mutable te::gm::Coord2D m_withinVtx0;
        mutable te::gm::Coord2D m_withinVtx1;        

    };

  } // end namespace rst
}   // end namespace te

#endif  // __TERRALIB_RASTER_INTERNAL_TILEINDEXER_H
