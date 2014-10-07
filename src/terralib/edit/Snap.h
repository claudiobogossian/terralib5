/*  Copyright (C) 2008-2011 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/edit/Snap.h

  \brief This class implements geometry snap concept.
*/

#ifndef __TERRALIB_EDIT_INTERNAL_SNAP_H
#define __TERRALIB_EDIT_INTERNAL_SNAP_H

// TerraLib
#include "../sam/rtree/Index.h"
#include "../srs/Config.h"
#include "Config.h"

// STL
#include <vector>
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
    struct Coord2D;
    class Envelope;
    class Geometry;
  }

  namespace edit
  {
    /*!
      \class Snap

      \brief This class implements geometry snap concept.
    */
    class TEEDITEXPORT Snap
    {
      public:

        Snap(const std::string& source, int srid = TE_UNKNOWN_SRS);

        ~Snap();

        void build(te::da::DataSet* dataset);

        void add(te::gm::Geometry* geom);

        bool search(const te::gm::Envelope& e, te::gm::Coord2D& coord);

        std::size_t getNGeometries() const;

        double getTolerance() const;

        void setTolerance(const double& t);

        void clear();

      private:

        std::string m_source;                          //!< The source of the geometries.
        int m_srid;                                    //!< The SRS of the geometries.
        std::size_t m_nGeometries;                     //!< The current number of geometries added to the snap.
        std::size_t m_maxGeometries;                   //!< The maximum number of geometries that can be added to the snap. If 0, there will be not limit.
        double m_tolerance;                            //!< The tolerance that will be used by the snap. For while, the unit is screen pixels.
        std::vector<te::gm::Coord2D> m_coords;         //!< The snap coordinates.
        te::sam::rtree::Index<std::size_t, 8> m_rtree; //!< Internal index used to retrieve geometries spatially.
    };

  } // end namespace edit
}   // end namespace te

#endif  // __TERRALIB_EDIT_INTERNAL_SNAP_H
