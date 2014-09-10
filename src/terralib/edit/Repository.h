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
  \file terralib/edt/Repository.h

  \brief This class represents a repository of geometries and features.
*/

#ifndef __TERRALIB_EDIT_INTERNAL_REPOSITORY_H
#define __TERRALIB_EDIT_INTERNAL_REPOSITORY_H

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
    class IdGeometry;

    /*!
      \class Repository

      \brief This class represents a repository of geometries and features.
    */
    class TEEDITEXPORT Repository
    {
      public:

        Repository(const std::string& source);

        ~Repository();

        void add(te::da::ObjectId* id, te::gm::Geometry* geom);

        void set(te::da::ObjectId* id, te::gm::Geometry* geom);

        void remove(te::da::ObjectId* id);

        std::size_t getPosition(te::da::ObjectId* id);

        bool hasIdentifier(te::da::ObjectId* id);

        const std::string& getSource() const;

        const std::vector<IdGeometry*>& getGeometries() const;

        std::vector<IdGeometry*> getGeometries(const te::gm::Envelope& e, int srid) const;

        IdGeometry* getGeometry(const te::gm::Envelope& e, int srid) const;

        void clear();

      private:

        void set(const std::size_t& pos, te::da::ObjectId* id, te::gm::Geometry* geom);

        void clearIndex();

        void buildIndex();

        void buildIndex(const std::size_t& pos, te::gm::Geometry* geom);

      private:

        std::string m_source;                          //!< The source of the identified geometries.
        std::vector<IdGeometry*> m_geoms;              //!< The identified geometries.
        te::sam::rtree::Index<std::size_t, 8> m_rtree; //!< Internal index used to retrieve geometries spatially.
    };

  } // end namespace edit
}   // end namespace te

#endif  // __TERRALIB_EDIT_INTERNAL_REPOSITORY_H
