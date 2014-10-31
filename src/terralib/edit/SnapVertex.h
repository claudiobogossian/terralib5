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
  \file terralib/edit/SnapVertex.h

  \brief This class implements a vertex search snap.
*/

#ifndef __TERRALIB_EDIT_INTERNAL_SNAPVERTEX_H
#define __TERRALIB_EDIT_INTERNAL_SNAPVERTEX_H

// TerraLib
#include "../sam/rtree/Index.h"
#include "Snap.h"

// STL
#include <vector>

namespace te
{
  namespace edit
  {
    /*!
      \class SnapVertex

      \brief This class implements a vertex search snap.
    */
    class TEEDITEXPORT SnapVertex : public Snap
    {
      public:

        SnapVertex(const std::string& source, int srid = TE_UNKNOWN_SRS);

        ~SnapVertex();

        void add(te::gm::Geometry* geom);

        void clear();

        std::string getName() const;

        std::string getDescription() const;

        static Snap* Builder(const std::string& source, int srid);

      protected:

        bool search(const te::gm::Envelope& e, te::gm::Coord2D& result);

      private:

        std::vector<te::gm::Coord2D> m_coords;          //!< The snap coordinates.
        te::sam::rtree::Index<std::size_t, 8> m_rtree;  //!< Internal index used to retrieve geometries spatially.
    };

  } // end namespace edit
}   // end namespace te

#endif  // __TERRALIB_EDIT_INTERNAL_SNAPVERTEX_H
