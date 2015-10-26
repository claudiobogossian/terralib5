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
  \file terralib/maptools/MarkRendererManager.h

  \brief This is a singleton for managing all mark renderers instances available in the system.
*/

#ifndef __TERRALIB_MAPTOOLS_INTERNAL_MARKRENDERERMANAGER_H
#define __TERRALIB_MAPTOOLS_INTERNAL_MARKRENDERERMANAGER_H

// TerraLib
#include "../color/RGBAColor.h"
#include "../common/Singleton.h"
#include "../common/ThreadingPolicies.h"
#include "Config.h"

// STL
#include <map>

namespace te
{
// Forward declaration
  namespace se
  {
    class Mark;
  }

  namespace map
  {
// Forward declaration
    class AbstractMarkRenderer;

    /*!
      \class MarkRendererManager

      \brief This is a singleton for managing all mark renderers instances available in the system.

      If you want to render a mark, use commands like:
      <code>
      te::color::RBGA** image = te::map::MarkRendererManager::getInstance().render(mark, size);
      </code>

      \sa AbstractMarkRenderer
    */
    class TEMAPEXPORT MarkRendererManager : public te::common::ClassLevelLockable<MarkRendererManager,
                                                                                 ::boost::recursive_mutex,
                                                                                 ::boost::lock_guard< ::boost::recursive_mutex>,
                                                                                 ::boost::lock_guard< ::boost::recursive_mutex> >,
                                            public te::common::Singleton<MarkRendererManager>
    {
      friend class te::common::Singleton<MarkRendererManager>;

      public:

        /*!
          \brief It generates the image pattern from the given Symbology Enconding Mark element.

          \param mark The Symbology Enconding Mark element that will be used as base to build the image pattern.
          \param size The size of image pattern that will be generated.

          \return The RGBA image that represents the mark.

          \note This method will look the name of the mark and try extract the associated rendererKey.
                The pattern used is "rendererKey://markName".
                Case empty will be used the default mark renderer.

          \note The caller will take the ownership of the returned pointer.

          \exception Exception It will throws an exception if the image pattern can not be generated.
        */
        te::color::RGBAColor** render(const te::se::Mark* mark, std::size_t size);

        /*!
          \brief It informs the set of all supported marks available in the system.

          \param marks A pre-created vector of string that will be filled with the supported marks names.

          \note All registered mark renderers will be consulted.
        */
        void getAllSupportedMarks(std::vector<std::string>& marks) const;

        /*!
          \brief It adds a new mark renderer to the manager.

          \param key      The key of the new renderer.
          \param renderer The new mark renderer that will be added on the manager.

          \note The manager will take the owership of the given mark renderer.

          \exception Exception It will throws an exception if there is already a mark renderer registered with the given key.
        */
        void add(const std::string& key, AbstractMarkRenderer* renderer);

        /*!
        \brief It clears the marks renderers of the manager.

        */
        void clear();

     protected:

        /*! \brief It initializes the singleton instance of the mark renderer manager. */
        MarkRendererManager();

        /*! \brief Singleton destructor. */
        ~MarkRendererManager();

      private:

        std::map<std::string, AbstractMarkRenderer*> m_renderers; //!< The mark renderers in the manager.
    };

  } // end namespace map
}   // end namespace te

#endif  // __TERRALIB_MAPTOOLS_INTERNAL_MARKRENDERERMANAGER_H
