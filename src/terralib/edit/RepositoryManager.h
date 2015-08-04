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
  \file terralib/edit/RepositoryManager.h

  \brief This is a singleton for managing edit repositories.
*/

#ifndef __TERRALIB_EDIT_INTERNAL_REPOSITORYMANAGER_H
#define __TERRALIB_EDIT_INTERNAL_REPOSITORYMANAGER_H

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
    class Feature;
    class Repository;
    /*!
      \class RepositoryManager

      \brief This is a singleton for managing edit repositories.
    */
    class TEEDITEXPORT RepositoryManager
    {
      
      public:

        /*! \brief It initializes the singleton instance of the repository manager. */
        RepositoryManager();

        /*! \brief Singleton destructor. */
        ~RepositoryManager();

        void addGeometry(const std::string& source, te::gm::Geometry* geom);

        void addGeometry(const std::string& source, te::da::ObjectId* id, te::gm::Geometry* geom);

        void addFeature(const std::string& source, Feature* f);

        bool hasIdentify(const std::string& source, te::da::ObjectId* id);

        const std::map<std::string, Repository*>& getRepositories() const;

        Repository* getRepository(const std::string& source) const;

        std::vector<Feature*> getFeatures(const std::string& source, const te::gm::Envelope& e, int srid) const;

        Feature* getFeature(const std::string& source, const te::gm::Envelope& e, int srid) const;

        void clearAll();

        void clear(const std::string& source);

        void removeAll();

        void remove(const std::string& source);

        void removeFeature(const std::string& source, te::da::ObjectId* id);

      private:

        std::map<std::string, Repository*> m_repositories;
    };

  } // end namespace edit
}   // end namespace te

#endif  // __TERRALIB_EDIT_INTERNAL_REPOSITORYMANAGER_H
