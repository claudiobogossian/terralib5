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
  \file terralib/edit/SnapManager.h

  \brief This is a singleton for managing geometries snap.
*/

#ifndef __TERRALIB_EDIT_INTERNAL_SNAPMANAGER_H
#define __TERRALIB_EDIT_INTERNAL_SNAPMANAGER_H

// TerraLib
#include "../common/Singleton.h"
#include "Config.h"

// STL
#include <map>
#include <vector>

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
    class Geometry;
  }

  namespace edit
  {
// Forward declaration
    class Snap;

    /*!
      \class SnapManager

      \brief This is a singleton for managing geometries snap.
    */
    class TEEDITEXPORT SnapManager : public te::common::Singleton<SnapManager>
    {
      friend class te::common::Singleton<SnapManager>;

      public:

        bool hasSnap(const std::string& source) const;

        void createSnap(const std::string& source, int srid);

        void buildSnap(const std::string& source, int srid, te::da::DataSet* dataset);

        void removeSnap(const std::string& source);

        const std::map<std::string, Snap*>& getSnaps() const;

        Snap* getSnap(const std::string& source) const;

        void clear(const std::string& source);

        void clearAll();

        bool search(const te::gm::Coord2D& coord, te::gm::Coord2D& result);

        void setWorld(const double& llx, const double& lly,
                      const double& urx, const double& ury,
                      const std::size_t& width, const std::size_t& height);

      protected:

        /*! \brief It initializes the singleton instance of the snap manager. */
        SnapManager();

        /*! \brief Singleton destructor. */
        ~SnapManager();

      private:

        std::map<std::string, Snap*> m_snaps;
    };

  } // end namespace edit
}   // end namespace te

#endif  // __TERRALIB_EDIT_INTERNAL_SNAPMANAGER_H
