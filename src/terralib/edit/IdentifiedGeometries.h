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
  \file terralib/edt/IdentifiedGeometries.h

  \brief This is a class for identifying edited geometries.
*/

#ifndef __TERRALIB_EDIT_INTERNAL_IDENTIFIEDGEOMETRIES_H
#define __TERRALIB_EDIT_INTERNAL_IDENTIFIEDGEOMETRIES_H

// TerraLib
#include "../sam/rtree/Index.h"
#include "Config.h"

// STL
#include <vector>
#include <string>

namespace te
{
// Forward declaration
  namespace da
  {
    class ObjectId;
  }

  namespace gm
  {
    class Envelope;
    class Geometry;
  }

  namespace edit
  {
// Forward declaration
    struct IdGeom;

    /*!
      \class IdentifiedGeometries

      \brief This is a class for identifying edited geometries.
    */
    class TEEDITEXPORT IdentifiedGeometries
    {
      public:

        IdentifiedGeometries(const std::string& source);

        ~IdentifiedGeometries();

        void add(te::da::ObjectId* id, te::gm::Geometry* geom);

        void set(te::da::ObjectId* id, te::gm::Geometry* geom);

        void remove(te::da::ObjectId* id);

        std::size_t getIdentifierPos(te::da::ObjectId* id);

        bool hasIdentifier(te::da::ObjectId* id);

        const std::string& getSource() const;

        const std::vector<te::da::ObjectId*>& getIdentifiers() const;

        const std::vector<te::gm::Geometry*>& getGeometries() const;

        std::vector<IdGeom*> getGeometries(const te::gm::Envelope& e, int srid) const;

        IdGeom* getGeometry(const te::gm::Envelope& e, int srid) const;

        void clear();

      private:

        void set(const std::size_t& pos, te::da::ObjectId* id, te::gm::Geometry* geom);

        void clearIndex();

        void buildIndex();

        void buildIndex(const std::size_t& pos, te::gm::Geometry* geom);

      private:

        std::string m_source;                          //!< The source from the identifiers and geometries.
        std::vector<te::da::ObjectId*> m_ids;          //!< The geometries identifiers.
        std::vector<te::gm::Geometry*> m_geoms;        //!< The indetified geometries.
        te::sam::rtree::Index<std::size_t, 8> m_rtree; //!< Internal index used to retrieve geometries spatially.
    };

  } // end namespace edit
}   // end namespace te

#endif  // __TERRALIB_EDIT_INTERNAL_IDENTIFIEDGEOMETRIES_H
